
#include <glm/gtc/matrix_transform.hpp>

#include "animations/scale.h"

namespace abeat::animation {

Scale::Scale(const Config &config):
	Animation(config),
	stream_input(config.input) {}

glm::mat4 Scale::get_matrix() const {
	auto avg = stream_input->get_average();
	return glm::scale(glm::mat4(1.f), glm::vec3(avg));
}

} // namespace abeat::animation
