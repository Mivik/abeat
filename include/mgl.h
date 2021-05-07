
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace mgl {

#define OBJ(name) \
    name(name &&t) noexcept: id(t.id) { t.id = 0; } \
    name(const name &) = delete; \
    name &operator=(name &&) noexcept = default;

// Vertex Array Object
struct VAO {
	VAO() { glGenVertexArrays(1, &id); }
	~VAO() { glDeleteVertexArrays(1, &id); }
	OBJ(VAO)

	inline void bind() const { glBindVertexArray(id); }
	static inline void unbind() { glBindVertexArray(0); }

	GLuint id;
};

// Frame Buffer Object
struct FBO {
	FBO() { glGenFramebuffers(1, &id); }
	~FBO() { glDeleteFramebuffers(1, &id); }
	OBJ(FBO)

	inline void bind(GLenum target = GL_FRAMEBUFFER) { glBindFramebuffer(target, id); }
	static inline void unbind(GLenum target = GL_FRAMEBUFFER) { glBindFramebuffer(target, 0); }

	GLuint id;
};

struct Buffer {
	Buffer() { glGenBuffers(1, &id); }
	~Buffer() { glDeleteBuffers(1, &id); }
	OBJ(Buffer)

	inline void bind(int type = GL_ARRAY_BUFFER) const {
		if (type == GL_TRANSFORM_FEEDBACK_BUFFER)
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, id);
		else
			glBindBuffer(type, id);
	}
	static inline void unbind(int type = GL_ARRAY_BUFFER) { glBindBuffer(type, 0); }

	GLuint id;
};

struct Texture {
	Texture() { glGenTextures(1, &id); }
	~Texture() { glDeleteTextures(1, &id); }
	OBJ(Texture)

	inline void bind(int type = GL_TEXTURE_2D) const { glBindTexture(type, id); }
	static inline void unbind(int type = GL_TEXTURE_2D) { glBindTexture(type, 0); }

	static void load_file(const std::string &path);

	GLuint id;
};

struct Shader {
	Shader(const char *code, GLuint type);
	~Shader() { if (id) glDeleteShader(id); }
	OBJ(Shader)

	GLuint id;
};

struct Program {
	Program();
	~Program() { if (id) glDeleteProgram(id); }
	OBJ(Program)

	inline void attach(const Shader &shader) { glAttachShader(id, shader.id); }
	template<typename...Args>
	inline void attach(const Shader &sh, const Args &... shs) {
		attach(sh);
		attach(shs...);
	};
	inline void detach(const Shader &shader) { glDetachShader(id, shader.id); }
	template<typename...Args>
	inline void detach(const Shader &sh, const Args &... shs) {
		detach(sh);
		detach(shs...);
	};

	template<typename...Args>
	void link(const Args &... shs) {
		attach(shs...);
		glLinkProgram(id);
		detach(shs...);
		check_link();
	}

	void check_link() const;

	inline GLuint locate(const char *name) const { return glGetUniformLocation(id, name); }
	inline void use() const { glUseProgram(id); }

	GLuint id;
};

// Multisample Anti-Aliasing
class MSAA {
public:
	MSAA(GLsizei samples, GLsizei width, GLsizei height);

	void blit();
	void resize(GLsizei new_width, GLsizei new_height);
	inline void bind() { fbo.bind(); }
private:
	FBO fbo;
	Texture texture;
	GLsizei samples, width, height;
};

struct Color { float r, g, b, a; };

#undef OBJ

} // namespace mgl

#define EX_COLOR(c) c.r, c.g, c.b, c.a
