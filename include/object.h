
#pragma once

#include "config.h"
#include "context.h"

namespace abeat {

class Object {
public:
	struct Config {
	};

	Object(const Object &) = delete;

	virtual ~Object() = default;

	virtual void update(float dt) {}

protected:
	explicit Object(const Config &config);
};

} // namespace abeat
