
#pragma once

#include <functional>

#include "stream.h"

namespace abeat::stream {

class Transform : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		std::function<float(float)> function;
	};

	class Builder;

	explicit Transform(const Config &config);
	Transform(const Transform &) = delete;
	~Transform() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return size; }

private:
	Stream *stream_input;
	size_t size;
	float *output;
	std::function<float(float)> function;
};

class Transform::Builder {
BUILDER_BEGIN(Transform)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(function)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
