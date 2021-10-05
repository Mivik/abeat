
#pragma once

#include "streams/pulse_audio.h"

namespace abeat::stream {

class Stereo : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		bool channel;
	};

	class Builder;

	explicit Stereo(const Config &config);
	Stereo(const Stereo &) = delete;
	~Stereo() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return input_size / 2; }
	[[nodiscard]] inline uint32_t get_sampling_rate() const { return stream_input->get_config().sampling_rate; }

private:
	PAInput *stream_input;
	size_t input_size;
	bool channel;
	float *output;
};

class Stereo::Builder {
BUILDER_BEGIN(Stereo)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(channel)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
