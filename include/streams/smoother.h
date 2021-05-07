
#pragma once

#include "stream.h"

namespace abeat::stream {

class Smoother : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		float scale = .15f;
	};

	class Builder;

	explicit Smoother(const Config &config);
	Smoother(const Smoother &) = delete;
	~Smoother() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output[1]; }
	[[nodiscard]] inline size_t get_output_size() const override { return size; }

private:
	Stream *stream_input;
	size_t size;
	float scale, *output[2];
};

class Smoother::Builder {
BUILDER_BEGIN(Smoother)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(scale)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
