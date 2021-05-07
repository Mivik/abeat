
#pragma once

#include <cstdint>

#include <fftw3.h>

#include "buffer.h"
#include "stream.h"

namespace abeat::stream {

class FFT : public Stream {
public:
	using WindowFuncType = void(float *, size_t);

	class Window {
	public:
		static WindowFuncType Blackman;
		static WindowFuncType Hamming;
		static WindowFuncType Hann;
		static WindowFuncType Nuttall;
		static WindowFuncType Rect;
		Window() = delete;
	};

	struct Config : public Stream::Config {
		Stream *input;
		size_t size = 4096;
		WindowFuncType *window = Window::Nuttall;
	};

	class Builder;

	explicit FFT(const Config &config);
	FFT(const FFT &) = delete;
	~FFT() override;

	void update(float dt) override;

	[[nodiscard]] const float *get_output() const override { return stream_output; }
	[[nodiscard]] inline size_t get_output_size() const override { return size / 2 + 1; }

	[[nodiscard]] inline size_t get_window_size() const { return std::min(size, input_size); }
	[[nodiscard]] inline float get_sampling_rate() const { return sampling_rate; }

private:
	Stream *stream_input;
	size_t size, input_size;
	fftwf_complex *output;
	float *input, *window;
	fftwf_plan plan;
	WindowFuncType *window_func;
	float sampling_rate, *stream_output;
};

class FFT::Builder {
BUILDER_BEGIN(FFT)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(size)
	BUILDER_OPT(window)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
