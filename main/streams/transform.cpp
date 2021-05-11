
#include <cmath>

#include "streams/transform.h"

namespace abeat::stream {

Transform::Transform(const Config &config)
	: Stream(config),
	  stream_input(config.input),
	  size(stream_input->get_output_size()),
	  function(config.function),
	  output(alloc_filled<float>(size, config.initial_value)) {}

Transform::~Transform() {
	delete[] output;
}

void Transform::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	for (size_t i = size - 1; ~i; --i) output[i] = function(input[i]);
}

} // namespace abeat::stream
