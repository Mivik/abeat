
#include <glm/gtc/type_ptr.hpp>

#include "widgets/bars.h"
#include "widgets/bars/shaders.h"

namespace abeat::widget {

Bars::Bars(const Config &config):
	Widget(config),
	stream_input(config.input) {
	const auto size = stream_input->get_output_size();
	{
		mgl::Shader
			vertex_shader(SHADER_VERT_BAR, GL_VERTEX_SHADER),
			geometry_shader(SHADER_GEOM_BAR, GL_GEOMETRY_SHADER),
			fragment_shader(SHADER_FRAG_CURVE, GL_FRAGMENT_SHADER);
		p_bar.link(vertex_shader, geometry_shader, fragment_shader);
		p_bar.use();
		glUniform1i(p_bar.locate("u_count"), size);
		glUniform1f(p_bar.locate("u_half"), config.width / size);
		glUniform4f(p_bar.locate("u_color"), EX_COLOR(config.color));
	}
	b_bar.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, nullptr, GL_STREAM_DRAW);
	v_bar.bind();
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	mgl::VAO::unbind();
	mgl::Buffer::unbind();
}

void Bars::update(float dt) {
	Widget::update(dt);

	const auto size = stream_input->get_output_size();

	b_bar.bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, stream_input->get_output());
	mgl::Buffer::unbind();

	p_bar.use();
	v_bar.bind();
	glUniformMatrix4fv(p_bar.locate("u_transform"), 1, GL_FALSE, glm::value_ptr(get_transform_matrix()));
	glDrawArrays(GL_POINTS, 0, size);
	mgl::VAO::unbind();
}

} // namespace abeat::widget
