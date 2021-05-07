
#include <stdexcept>

#include "stb_image.h"

#include "mgl.h"

namespace mgl {

void Texture::load_file(const std::string &path) {
	int width, height, channels;
	auto *data = stbi_load(path.data(), &width, &height, &channels, 0);
	if (!data) throw std::invalid_argument("Failed to load image from " + path);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

Shader::Shader(const char *code, GLuint type) {
	if (!(id = glCreateShader(type)))
		throw std::runtime_error("Failed to create shader");
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::string str;
		str.resize(length);
		glGetShaderInfoLog(id, length, &length, str.data());
		throw std::invalid_argument(str);
	}
}

Program::Program() {
	if (!(id = glCreateProgram()))
		throw std::runtime_error("Failed to create shader program");
}

void Program::check_link() const {
	GLint link_status;
	glGetProgramiv(id, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) {
		GLint length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		std::string log;
		log.resize(length);
		glGetProgramInfoLog(id, length, &length, log.data());
		throw std::invalid_argument(log);
	}
}

MSAA::MSAA(GLsizei samples, GLsizei width, GLsizei height):
	samples(samples),
	width(0),
	height(0) {
	resize(width, height);
	fbo.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture.id, 0);
	FBO::unbind();
}

void MSAA::blit() {
	fbo.bind(GL_READ_FRAMEBUFFER);
	FBO::unbind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	FBO::unbind();
}

void MSAA::resize(GLsizei new_width, GLsizei new_height) {
	if (width == new_width && height == new_height) return;
	width = new_width;
	height = new_height;
	texture.bind(GL_TEXTURE_2D_MULTISAMPLE);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA8, width, height, GL_TRUE);
	Texture::unbind(GL_TEXTURE_2D_MULTISAMPLE);
}

} // namespace mgl
