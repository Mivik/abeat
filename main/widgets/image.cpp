
#include "widgets/image.h"
#include "widgets/image/shaders.h"

namespace abeat::widget {

inline float VERTEX_DATA[] = {
	0, 0,
	1, 0,
	0, 1,
	1, 1
};

Image::Image(const Config &config):
	Widget(config) {
	texture.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap_x);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap_y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.mag_filter);
	mgl::Texture::load_file(config.path);
	mgl::Texture::unbind();

	VAO.bind();
	VBO.bind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	mgl::Buffer::unbind();
	mgl::VAO::unbind();

	{
		mgl::Shader
			vertex_shader(SHADER_VERT_IMAGE, GL_VERTEX_SHADER),
			fragment_shader(SHADER_FRAG_IMAGE, GL_FRAGMENT_SHADER);
		prog.link(vertex_shader, fragment_shader);
		prog.use();
	}
}

void Image::update(float dt) {
	Widget::update(dt);

	prog.use();

	texture.bind();
	VAO.bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	mgl::VAO::unbind();
}

} // namespace abeat::widget
