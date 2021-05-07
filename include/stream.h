
#pragma once

#include <cstddef>
#include <map>

#include "common.h"
#include "object.h"

#define STREAM_BUILDER

namespace abeat {

class Stream : public Object {
public:
	struct Config : public Object::Config {
	};

	Stream(const Stream &) = delete;

	void update(float dt) override;

	[[nodiscard]] virtual const float *get_output() const { return nullptr; }
	[[nodiscard]] virtual size_t get_output_size() const { return 0; }

	[[nodiscard]] float get_average() const;

protected:
	explicit Stream(const Config &config);
};

} // namespace abeat
