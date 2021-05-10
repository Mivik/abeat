
#include <cmath>

#include "streams/smoother.h"

namespace abeat::stream {

Smoother::Smoother(const Config &config):
	Stream(config),
	stream_input(config.input),
	size(stream_input->get_output_size()),
	scale(config.scale) {
	if (scale < 0 || scale > 1) throw std::invalid_argument("Scale should be a real number in [0, 1]");
	output[0] = alloc_filled<float>(size, config.initial_value);
	output[1] = alloc_filled<float>(size, config.initial_value);
}

Smoother::~Smoother() {
	delete[] output[0];
	delete[] output[1];
}

void Smoother::update(float dt) {
	Stream::update(dt);

	std::swap(output[0], output[1]);
	auto *input = stream_input->get_output();
	for (size_t i = size - 1; ~i; --i) {
		const float pre = output[0][i];
		if (std::isnormal(pre)) output[1][i] = pre + (input[i] - pre) * scale;
		else output[1][i] = input[i];
	}
}

} // namespace abeat::stream
