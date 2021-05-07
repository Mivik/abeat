
#pragma once

#include <memory>

#include <pulse/pulseaudio.h>

#include "buffer.h"
#include "stream.h"

namespace abeat::stream {

struct PAInput : public Stream {
public:
	struct Config : public Stream::Config {
		uint32_t sampling_rate = 44100;
		uint32_t latency = 1100;
		uint32_t duration = 50; // ms
		bool stereo = false;

		[[nodiscard]] inline size_t get_buffer_size() const {
			auto t = (size_t) sampling_rate * duration / 1000;
			return stereo? t * 2: t;
		}
	};

	class Builder;

	explicit PAInput(const Config &config);
	PAInput(PAInput &) = delete;
	~PAInput() override;

	void start();
	void stop();

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return stream_output; }
	[[nodiscard]] inline size_t get_output_size() const override { return buffer.get_size(); }

	[[nodiscard]] inline Config get_config() const { return config; }

private:
	static void context_state_callback(pa_context *context, void *userdata);
	static void context_info_callback(pa_context *context, const pa_server_info *info, void *userdata);
	static void stream_state_callback(pa_stream *stream, void *userdata);
	static void stream_read_callback(pa_stream *stream, size_t length, void *userdata);

	Config config;
	std::string device;
	pa_threaded_mainloop *mainloop;
	pa_context *context;
	pa_stream *stream;
	Buffer<int16_t> buffer;
	float *stream_output;
};

class PAInput::Builder {
BUILDER_BEGIN(PAInput)
	STREAM_BUILDER
	BUILDER_OPT(sampling_rate)
	BUILDER_OPT(latency)
	BUILDER_OPT(duration)
	BUILDER_OPT(stereo)
BUILDER_END(true)
};

} // namespace abeat::stream
