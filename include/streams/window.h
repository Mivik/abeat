
#pragma once

#include "stream.h"

namespace abeat::stream {

class Window : public Stream {
public:
	struct Config : public Stream::Config {
		Stream *input;
		Range<float> from_range, to_range = { 0, 1 };
	};

	class Builder;

	explicit Window(Config config);
	Window(const Window &) = delete;
	~Window() override;

	void update(float dt) override;

	[[nodiscard]] inline const float *get_output() const override { return output; }
	[[nodiscard]] inline size_t get_output_size() const override { return size; }

private:
	Config config;
	Stream *stream_input;
	size_t size;
	float *output;
};

class Window::Builder {
BUILDER_BEGIN(Window)
	STREAM_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(from_range)
	BUILDER_OPT(to_range)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
