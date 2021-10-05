
#include "widget.h"

namespace abeat {

Widget::Widget(const Config &config):
	Object(config),
	basic_transform(config.transform),
	animations(config.animations) {}

void Widget::update(float dt) {
	animation_transform = glm::mat4(1.f);
	for (auto *animation : animations)
		animation_transform *= animation->get_matrix();
}

} // namespace abeat
