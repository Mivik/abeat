
#pragma once

#include "mgl.h"
#include "widget.h"
#include "stream.h"

namespace abeat::widget {

class Bars : public Widget {
public:
	struct Config : public Widget::Config {
		Stream *input;
		float width = .6f;
		mgl::Color color = Widget::DEFAULT_COLOR;
	};

	class Builder;

	explicit Bars(const Config &config);
	Bars(const Bars &) = delete;

	void update(float dt) override;

private:
	Stream *stream_input;
	mgl::Buffer b_bar;
	mgl::VAO v_bar;
	mgl::Program p_bar;
};

class Bars::Builder {
BUILDER_BEGIN(Bars)
	WIDGET_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(width)
	BUILDER_OPT(color)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::widget
