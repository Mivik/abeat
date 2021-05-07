
#pragma once

#include "streams/FFT.h"

namespace abeat::stream {

class Spectrum : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		Range<float> freq = { 0, 1500 }; // Hz
	};

	class Builder;

	explicit Spectrum(const Config &config);
	Spectrum(const Spectrum &) = delete;
	~Spectrum() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return output_size; }

private:
	float *output;
	FFT *stream_input;
	size_t offset, output_size;
};

class Spectrum::Builder {
BUILDER_BEGIN(Spectrum)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(freq)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
