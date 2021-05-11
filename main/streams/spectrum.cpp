
#include <cmath>

#include "streams/spectrum.h"

namespace abeat::stream {

Spectrum::Spectrum(const Config &config):
	Stream(config),
	stream_input(dynamic_cast<FFT *>(config.input)) {
	if (!stream_input)
		throw std::invalid_argument("Input stream is not a FFT");
	const auto input_size = stream_input->get_output_size();
	const float density = (float) stream_input->get_sampling_rate() / input_size / 2;
	offset = (float) config.freq.from / density;
	output_size =
		std::min((size_t) std::ceil((float) config.freq.to / density) + 1, input_size) - offset;
	output = alloc_filled<float>(output_size, config.initial_value);
}

Spectrum::~Spectrum() {
	delete[] output;
}

void Spectrum::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	for (size_t i = 0; i < output_size; ++i)
		output[i] = 20 * std::log10(input[i]);
}

} // namespace abeat::stream
