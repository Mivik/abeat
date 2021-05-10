
#include <cmath>

#include "streams/pow.h"

namespace abeat::stream {

Pow::Pow(const Config &config)
	: Stream(config),
	  stream_input(config.input),
	  size(stream_input->get_output_size()),
	  power(config.power),
	  output(alloc_filled<float>(size, config.initial_value)) {}

Pow::~Pow() {
	delete[] output;
}

void Pow::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	for (size_t i = size - 1; ~i; --i) output[i] = std::pow(input[i], power);
}

} // namespace abeat::stream
