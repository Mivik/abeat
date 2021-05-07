
#pragma once

#include "mgl.h"
#include "widget.h"

namespace abeat::widget {

class Curve : public Widget {
public:
	struct Config : public Widget::Config {
		Stream *input;
		uint16_t sample_num = 4;
		mgl::Color color = Widget::DEFAULT_COLOR;
		bool circular = false, filled = false;
		float line_width = 2.f, radius = .3f;
	};

	class Builder;

	explicit Curve(const Config &config);
	Curve(const Curve &) = delete;

	void update(float dt) override;

private:
	Stream *stream_input;
	mgl::Program p_curve, p_join;
	mgl::Buffer b_curve;
	mgl::VAO v_curve;
	bool circular, filled;
	float line_width;
};

class Curve::Builder {
BUILDER_BEGIN(Curve)
	WIDGET_BUILDER
	BUILDER_ARG(input)

	BUILDER_OPT(sample_num)
	BUILDER_OPT(color)
	BUILDER_OPT(circular)
	BUILDER_OPT(filled)
	BUILDER_OPT(line_width)
	BUILDER_OPT(radius)
BUILDER_END(f_input)
	bool f_input = false;
};

} // namespace abeat::widget
