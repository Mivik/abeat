
#pragma once

#include <map>
#include <string>

#include <glm/glm.hpp>

#include "common.h"
#include "object.h"

namespace abeat {

class Animation : public Object {
public:
	struct Config : public Object::Config {
	};

	Animation(const Animation &) = delete;

	void update(float dt) override;

	[[nodiscard]] virtual glm::mat4 get_matrix() const { return glm::mat4(1); }

protected:
	explicit Animation(const Config &config);
};

} // namespace abeat
