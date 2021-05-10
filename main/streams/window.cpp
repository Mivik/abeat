
#include <algorithm>

#include "streams/window.h"

namespace abeat::stream {

Window::Window(Config config)
	: Stream(config),
	  config(config),
	  stream_input(this->config.input),
	  size(stream_input->get_output_size()),
	  output(alloc_filled<float>(size, config.initial_value)) {}

Window::~Window() {
	delete[] output;
}

void Window::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	for (size_t i = size - 1; ~i; --i) {
		output[i] =
			std::clamp(input[i] - config.from_range.min, 0.f, config.from_range.length());
		output[i] = output[i] * config.to_range.length() / config.from_range.length() + config.to_range.min;
	}
}

} // namespace abeat::stream
