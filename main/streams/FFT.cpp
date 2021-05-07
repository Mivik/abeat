
#include <iostream>

#include <cmath>

#include "streams/FFT.h"
#include "streams/pulse_audio.h"
#include "streams/stereo.h"

namespace abeat::stream {

FFT::FFT(const Config &config):
	Stream(config),
	stream_input(config.input),
	size(config.size),
	input_size(stream_input->get_output_size()),
	output(nullptr),
	input(nullptr),
	window(nullptr),
	plan(nullptr),
	window_func(config.window) {
	if (auto pa = dynamic_cast<PAInput *>(stream_input))
		sampling_rate = pa->get_config().sampling_rate;
	else if (auto stereo = dynamic_cast<Stereo *>(stream_input))
		sampling_rate = stereo->get_sampling_rate();
	else throw std::invalid_argument("Input of FFT should be an audio stream");
	input = fftwf_alloc_real(size);
	output = fftwf_alloc_complex(size / 2 + 1);
	plan = fftwf_plan_dft_r2c_1d(size, input, output, FFTW_ESTIMATE);
	stream_output = alloc_zero<float>(size);
	const auto ws = get_window_size();
	window = new float[ws];
	window_func(window, ws);
}

FFT::~FFT() {
	if (plan) fftwf_destroy_plan(plan);
	if (input) fftwf_free(input);
	if (output) fftwf_free(output);
	delete[] stream_output;
	delete[] window;
}

void FFT::update(float dt) {
	Stream::update(dt);

	const auto ws = get_window_size();
	auto *arr = stream_input->get_output() + input_size - ws;
	for (size_t i = 0; i < ws; ++i)
		input[i] = (float) arr[i] * window[i];
	std::fill(input + ws, input + size, 0);
	fftwf_execute(plan);
	const float scale = ws / 2.f * 32768;
	for (size_t i = get_output_size() - 1; ~i; --i)
		stream_output[i] = std::hypot(output[i][0], output[i][1]) / scale;
}

template<class T>
inline T sqr(T x) { return x * x; }

void FFT::Window::Blackman(float *window, size_t size) {
	const float c1 = 220.f / 189.f, c2 = 715.f / 3969.f;
	const float inv = 1.f / (float) (size - 1);
	for (size_t i = 0; i < size; ++i)
		window[i] = 1.f - c1 * std::cos(2 * (float) M_PI * i * inv) + c2 * std::cos(4 * (float) M_PI * i * inv);
}

void FFT::Window::Hamming(float *window, size_t size) {
	const float a0 = 21.f / 25;
	const float inv = 1.f / (float) size;
	for (size_t i = 0; i < size; ++i)
		window[i] = 1 - a0 * std::cos(2 * (float) M_PI * i * inv);
}

void FFT::Window::Hann(float *window, size_t size) {
	const float inv = 1.f / (float) size;
	for (size_t i = 0; i < size; ++i)
		window[i] = sqr(std::sin((float) M_PI * i * inv));
}

void FFT::Window::Nuttall(float *window, size_t size) {
	const float a0 = 0.355768f, a1 = 0.487396f, a2 = 0.144232f, a3 = 0.012604f;
	const float inv = 1.f / (float) size;
	for (size_t i = 0; i < size; ++i)
		window[i] = 1
					- a1 / a0 * std::cos(2 * (float) M_PI * i * inv)
					+ a2 / a0 * std::cos(4 * (float) M_PI * i * inv)
					- a3 / a0 * std::cos(6 * (float) M_PI * i * inv);
}

void FFT::Window::Rect(float *window, size_t size) { std::fill(window, window + size, 1.f); }

} // namespace abeat::stream
