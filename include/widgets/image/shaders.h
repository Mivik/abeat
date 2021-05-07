
#pragma once

constexpr const char *SHADER_VERT_IMAGE = R"(
#version 330

layout(location = 0) in vec2 i_pos;

out vec2 o_vertex;

void main() {
	gl_Position = vec4(mix(-1, 1, i_pos.x), mix(-1, 1, i_pos.y), 0, 1);

	o_vertex = i_pos;
}
)";

constexpr const char *SHADER_FRAG_IMAGE = R"(
#version 330

out vec4 o_color;

in vec2 o_vertex;

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, o_vertex);
}
)";
