
#include "stream.h"

namespace abeat {

Stream::Stream(const Config &config):
	Object(config) {}

void Stream::update(float dt) {}

float Stream::get_average() const {
	float avg = 0;
	auto *output = get_output();
	for (size_t i = get_output_size() - 1; ~i; --i) avg += output[i];
	return avg / get_output_size();
}

} // namespace abeat
