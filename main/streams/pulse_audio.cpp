
#include <cmath>

#include "config.h"

#include "streams/pulse_audio.h"

namespace abeat::stream {

struct Lock {
	explicit Lock(pa_threaded_mainloop *mainloop): mainloop(mainloop) { pa_threaded_mainloop_lock(mainloop); }
	inline ~Lock() { if (mainloop)pa_threaded_mainloop_unlock(mainloop); }
	inline void unlock() {
		if (mainloop) {
			pa_threaded_mainloop_unlock(mainloop);
			mainloop = nullptr;
		}
	}
private:
	pa_threaded_mainloop *mainloop;
};

PAInput::PAInput(const Config &config):
	Stream(config),
	config(config),
	buffer(config.get_buffer_size()) {
	if (!(mainloop = pa_threaded_mainloop_new()))
		throw std::runtime_error("Failed to create PA mainloop");
	{
		Lock lock(mainloop);
		context = pa_context_new(pa_threaded_mainloop_get_api(mainloop), "abeat");
		if (!context) {
			lock.unlock();
			pa_threaded_mainloop_free(mainloop);
			throw std::runtime_error("Failed to create PA context");
		}
		auto destroy = [&]() {
			pa_context_disconnect(context);
			pa_context_unref(context);
			lock.unlock();
			pa_threaded_mainloop_free(mainloop);
		};
		pa_context_set_state_callback(context, context_state_callback, this);
		if (pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr) < 0 ||
			pa_threaded_mainloop_start(mainloop) < 0) {
			destroy();
			throw std::runtime_error("Failed to connect PA context");
		}
		pa_threaded_mainloop_wait(mainloop);
		if (pa_context_get_state(context) != PA_CONTEXT_READY) {
			pa_threaded_mainloop_stop(mainloop);
			destroy();
			throw std::runtime_error("PA context no ready");
		}
		pa_operation *operation;
		operation = pa_context_get_server_info(context, context_info_callback, this);
		while (pa_operation_get_state(operation) != PA_OPERATION_DONE)
			pa_threaded_mainloop_wait(mainloop);
		pa_operation_unref(operation);

		stream_output = alloc_zero<float>(buffer.get_size());
	}
	start();
}

PAInput::~PAInput() {
	stop();
	pa_threaded_mainloop_stop(mainloop);
	pa_context_disconnect(context);
	pa_context_unref(context);
	pa_threaded_mainloop_free(mainloop);
	delete[] stream_output;
}

void PAInput::context_state_callback(pa_context *context, void *userdata) {
	auto *that = (PAInput *) userdata;
	switch (pa_context_get_state(context)) {
		case PA_CONTEXT_READY:
		case PA_CONTEXT_TERMINATED:
		case PA_CONTEXT_FAILED:
			pa_threaded_mainloop_signal(that->mainloop, 0);
			break;
		default:
			break;
	}
}

void PAInput::context_info_callback(pa_context *context, const pa_server_info *info, void *userdata) {
	auto *that = (PAInput *) userdata;
	that->device = info->default_sink_name;
	that->device += ".monitor";
	pa_threaded_mainloop_signal(that->mainloop, 0);
}

void PAInput::start() {
	Lock lock(mainloop);

	pa_sample_spec sample_spec = {};
	sample_spec.format = PA_SAMPLE_S16LE;
	sample_spec.rate = (uint32_t) config.sampling_rate;
	sample_spec.channels = (uint8_t) (config.stereo? 2: 1);

	stream = pa_stream_new(context, "abeat input", &sample_spec, nullptr);
	if (!stream) throw std::runtime_error("Failed to create PA stream");
	pa_stream_set_state_callback(stream, stream_state_callback, this);
	pa_stream_set_read_callback(stream, stream_read_callback, this);
	// TODO custom device
	pa_buffer_attr buffer_attr = {
		(uint32_t) -1,
		(uint32_t) -1,
		(uint32_t) -1,
		(uint32_t) -1,
		(uint32_t) config.sampling_rate,
	};
	if (pa_stream_connect_record(stream, device.data(), &buffer_attr, PA_STREAM_ADJUST_LATENCY) < 0)
		throw std::runtime_error("Failed to connect PA stream");
	pa_threaded_mainloop_wait(mainloop);
	if (pa_stream_get_state(stream) != PA_STREAM_READY)
		throw std::runtime_error("PA stream is not ready");
}

void PAInput::stream_state_callback(pa_stream *stream, void *userdata) {
	auto *that = (PAInput *) userdata;
	switch (pa_stream_get_state(stream)) {
		case PA_STREAM_READY:
		case PA_STREAM_TERMINATED:
		case PA_STREAM_FAILED:
			pa_threaded_mainloop_signal(that->mainloop, 0);
			break;
		default:
			break;
	}
}

void PAInput::stream_read_callback(pa_stream *stream, size_t length, void *userdata) {
	auto *that = (PAInput *) userdata;
	while (pa_stream_readable_size(stream) > 0) {
		int16_t *buf;
		if (pa_stream_peek(stream, (const void **) &buf, &length) < 0)
			return;
		if (buf)
			that->buffer.write(buf, length / sizeof(int16_t));
		pa_stream_drop(stream);
	}
}

void PAInput::stop() {
	if (!stream) return;
	Lock lock(mainloop);
	pa_stream_disconnect(stream);
	pa_stream_unref(stream);
	stream = nullptr;
}

void PAInput::update(float dt) {
	Stream::update(dt);

	auto lock = buffer.lock();
	if (!buffer.has_new_data) return;
	buffer.has_new_data = false;
	std::copy(buffer.data, buffer.data + buffer.get_size(), stream_output);
}

} // namespace abeat::stream
