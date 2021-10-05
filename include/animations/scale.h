
#pragma once

#include "animation.h"
#include "stream.h"

namespace abeat::animation {

class Scale : public Animation {
public:
	struct Config : public Animation::Config {
		Stream *input;
	};

	class Builder;

	explicit Scale(const Config &config);
	Scale(const Scale &) = delete;

	[[nodiscard]] glm::mat4 get_matrix() const override;

private:
	Stream *stream_input;
};

class Scale::Builder {
BUILDER_BEGIN(Scale)
	ANIMATION_BUILDER
	BUILDER_ARG(input)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::stream
