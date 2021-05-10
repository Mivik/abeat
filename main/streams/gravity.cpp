
#include "streams/gravity.h"

namespace abeat::stream {

Gravity::Gravity(const Config &config):
	Stream(config),
	stream_input(config.input),
	size(stream_input->get_output_size()),
	gravity(config.gravity),
	output(alloc_filled<float>(size, config.initial_value)),
	times(alloc_filled<float>(size, config.initial_value)) {
	if (gravity < 0) throw std::invalid_argument("Gravity should be positive");
}

Gravity::~Gravity() {
	delete[] output;
	delete[] times;
}

void Gravity::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	for (size_t i = size - 1; ~i; --i) {
		const float time = times[i];
		// RK4 integration
		float k1 = -gravity * time;
		float k2 = -gravity * (time + dt * .5f);
		float k4 = -gravity * (time + dt);
		float dy_dt = 1.f / 6.f * (k1 + 4.f * k2 + k4);

		float yg = output[i] + dy_dt * dt;

		if (input[i] >= yg) {
			output[i] = input[i];
			times[i] = 0;
		} else {
			output[i] = yg;
			times[i] = time + dt;
		}
	}
}

} // namespace abeat::stream
