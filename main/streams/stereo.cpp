
#include "streams/stereo.h"

namespace abeat::stream {

Stereo::Stereo(const Config &config):
	Stream(config),
	stream_input(dynamic_cast<PAInput *>(config.input)),
	input_size(stream_input->get_output_size()),
	channel(config.channel),
	output(alloc_filled<float>(stream_input->get_output_size() / 2, config.initial_value)) {
	if (!stream_input) throw std::invalid_argument("Input of a stereo should be an audio stream");
}

Stereo::~Stereo() {
	delete[] output;
}

void Stereo::update(float dt) {
	Stream::update(dt);

	auto *input = stream_input->get_output();
	const auto size = stream_input->get_output_size();
	for (size_t i = channel, j = 0; i < size; i += 2, ++j)
		output[j] = input[i];
}

} // namespace abeat::stream
