
#include <glm/gtc/type_ptr.hpp>

#include <fmt/format.h>

#include "stream.h"
#include "widgets/curve.h"
#include "widgets/curve/shaders.h"

namespace abeat::widget {

Curve::Curve(const Config &config):
	Widget(config),
	stream_input(config.input),
	circular(config.circular),
	filled(config.filled),
	line_width(config.line_width) {
	const auto size = stream_input->get_output_size();
	const char *vertex_shader_code = circular? SHADER_VERT_CIRCULAR_CURVE: SHADER_VERT_CURVE,
		*geometry_shader_code = circular?
								(filled? SHADER_GEOM_CIRCULAR_CURVE_FILLED: SHADER_GEOM_CIRCULAR_CURVE):
								(filled? SHADER_GEOM_CURVE_FILLED: SHADER_GEOM_CURVE);
	{
		mgl::Shader
			vertex_shader(vertex_shader_code, GL_VERTEX_SHADER),
			geometry_shader(
			(filled
			 ? fmt::format(geometry_shader_code, config.sample_num * (circular? 3: 6))
			 : fmt::format(geometry_shader_code, "line_strip", config.sample_num)
			).data(),
			GL_GEOMETRY_SHADER),
			fragment_shader(SHADER_FRAG_CURVE, GL_FRAGMENT_SHADER);
		p_curve.link(vertex_shader, geometry_shader, fragment_shader);
		p_curve.use();
		glUniform1i(p_curve.locate("u_count"), size);
		glUniform1f(p_curve.locate("u_radius"), config.radius);
		glUniform1i(p_curve.locate("u_sample_num"), config.sample_num);
		glUniform4f(p_curve.locate("u_color"), EX_COLOR(config.color));
	}
	if (!filled) {
		mgl::Shader
			vertex_shader(vertex_shader_code, GL_VERTEX_SHADER),
			geometry_shader(fmt::format(geometry_shader_code, "points", config.sample_num).data(), GL_GEOMETRY_SHADER),
			fragment_shader(SHADER_FRAG_DOTS, GL_FRAGMENT_SHADER);
		p_join.link(vertex_shader, geometry_shader, fragment_shader);
		p_join.use();
		glUniform1i(p_join.locate("u_count"), size);
		glUniform1f(p_join.locate("u_radius"), config.radius);
		glUniform1i(p_curve.locate("u_sample_num"), config.sample_num);
		glUniform4f(p_join.locate("u_color"), EX_COLOR(config.color));
	}
	b_curve.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size + circular * 2), nullptr, GL_STREAM_DRAW);
	v_curve.bind();
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	mgl::VAO::unbind();
	mgl::Buffer::unbind();
}

void Curve::update(float dt) {
	Widget::update(dt);

	const auto *input = stream_input->get_output();
	const auto size = stream_input->get_output_size();

	b_curve.bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, input);
	if (circular) glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * size, sizeof(float) * 2, input);
	mgl::Buffer::unbind();

	v_curve.bind();
	p_curve.use();
	glUniformMatrix4fv(p_curve.locate("u_transform"), 1, GL_FALSE, glm::value_ptr(get_transform_matrix()));
	glLineWidth(line_width);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, size + circular * 2);
	if (!filled) {
		p_join.use();
		glUniformMatrix4fv(p_join.locate("u_transform"), 1, GL_FALSE, glm::value_ptr(get_transform_matrix()));
		glPointSize(line_width);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, size + circular * 2);
	}
	mgl::VAO::unbind();
}

} // namespace abeat::widget
