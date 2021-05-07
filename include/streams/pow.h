
#pragma once

#include "stream.h"

namespace abeat::stream {

class Pow : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		float power;
	};

	class Builder;

	explicit Pow(const Config &config);
	Pow(const Pow &) = delete;
	~Pow() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return size; }

private:
	Stream *stream_input;
	size_t size;
	float power, *output;
};

class Pow::Builder {
BUILDER_BEGIN(Pow)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(power)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
