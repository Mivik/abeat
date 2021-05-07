
#pragma once

#include "mgl.h"
#include "widget.h"

namespace abeat::widget {

class Image : public Widget {
public:
	struct Config : public Widget::Config {
		std::string path;
		GLint wrap_x = GL_MIRRORED_REPEAT, wrap_y = GL_MIRRORED_REPEAT;
		GLint min_filter = GL_NEAREST, mag_filter = GL_LINEAR;
	};

	class Builder;

	explicit Image(const Config &config);
	Image(const Image &) = delete;

	void update(float dt) override;

private:
	mgl::Texture texture;
	mgl::Buffer VBO;
	mgl::VAO VAO;
	mgl::Program prog;
};

class Image::Builder {
BUILDER_BEGIN(Image)
	WIDGET_BUILDER
	BUILDER_ARG(path)

	BUILDER_OPT(wrap_x)
	BUILDER_OPT(wrap_y)
	BUILDER_OPT(min_filter)
	BUILDER_OPT(mag_filter)
BUILDER_END(f_path)
	bool f_path = false;
};

} // namespace abeat::widget
