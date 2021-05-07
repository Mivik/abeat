
#pragma once

#include "stream.h"

namespace abeat::stream {

class Gravity : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		float gravity = 8.f;
	};

	class Builder;

	explicit Gravity(const Config &config);
	Gravity(const Gravity &) = delete;
	~Gravity() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return size; }

private:
	Stream *stream_input;
	size_t size;
	float gravity, *output, *times;
};

class Gravity::Builder {
BUILDER_BEGIN(Gravity)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(gravity)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
