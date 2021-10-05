
#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "animation.h"
#include "common.h"
#include "mgl.h"
#include "object.h"

#define WIDGET_BUILDER \
    BUILDER_OPT(transform) \
    BUILDER_OPT(animations)

namespace abeat {

class Widget : public Object {
public:
	struct Config : public Object::Config {
		glm::mat4 transform = glm::mat4(1.f);
		std::vector<Animation *> animations;
	};

	static constexpr mgl::Color DEFAULT_COLOR = { 0x21 / 255.f, 0x96 / 255.f, 0xf3 / 255.f, 1.f };

	void update(float dt) override;

	[[nodiscard]] inline glm::mat4 get_transform_matrix() const {
		return animation_transform * basic_transform;
	}

protected:
	explicit Widget(const Config &config);
private:
	std::vector<Animation *> animations;
	glm::mat4 basic_transform{ 1.f };
	glm::mat4 animation_transform{ 1.f };
};

} // namespace abeat
