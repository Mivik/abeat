
#pragma once

constexpr const char *SHADER_VERT_BAR = R"(
#version 330

layout(location = 0) in float i_y;

uniform int u_count;

void main() {
	float x = mix(-1., 1., (float(gl_VertexID) + 0.5) / u_count);
	float y = mix(-1., 1., clamp(i_y, 0, 1));
	gl_Position = vec4(x, y, 0, 1);
}
)";

constexpr const char *SHADER_GEOM_BAR = R"(
#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float u_half;
uniform mat4 u_transform;

void main() {
	float l = gl_in[0].gl_Position.x - u_half;
	float r = gl_in[0].gl_Position.x + u_half;
	float y = gl_in[0].gl_Position.y;

	gl_Position = u_transform * vec4(l, -1, 0, 1); EmitVertex();
	gl_Position = u_transform * vec4(r, -1, 0, 1); EmitVertex();
	gl_Position = u_transform * vec4(l, y, 0, 1); EmitVertex();
	gl_Position = u_transform * vec4(r, y, 0, 1); EmitVertex();
	EndPrimitive();
}
)";

constexpr const char *SHADER_FRAG_CURVE = R"(
#version 330

out vec4 o_color;

uniform vec4 u_color;

void main() {
	o_color = u_color;
}
)";
