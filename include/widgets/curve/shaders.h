
#pragma once

constexpr const char *SHADER_VERT_CURVE = R"(
#version 330

layout(location = 0) in float i_y;

uniform int u_count;

out VERTEX_SHADER_OUT {
	int id;
} vs_out;

void main() {
	float x = mix(-1., 1., float(gl_VertexID) / (u_count - 1));
	float y = mix(-1., 1., clamp(i_y, 0, 1));
	gl_Position = vec4(x, y, 0, 1);
	vs_out.id = gl_VertexID;
}
)";

constexpr const char *SHADER_VERT_CIRCULAR_CURVE = R"(
#version 330

#define PI 3.1415926535897932384626433832795

layout(location = 0) in float i_y;

uniform int u_count;
uniform float u_radius;

out VERTEX_SHADER_OUT {
	int id;
} vs_out;

void main() {
	float angle = mix(0, PI * 2, float(gl_VertexID) / u_count);
	float dist = mix(u_radius, 1, clamp(i_y, 0, 1));
	gl_Position = vec4(cos(angle) * dist, sin(angle) * dist, 0, 1);
	vs_out.id = gl_VertexID;
}
)";

#define GET_POINTS R"(\
vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5);\
vec4 p1 = gl_in[1 - (vs_out[2].id & 1)].gl_Position;\
vec4 p2 = mix(p1, gl_in[2].gl_Position, 0.5);\
if (vs_out[0].id == 0) p0 = gl_in[0].gl_Position;\
if (vs_out[2].id == u_count - 1) p2 = gl_in[2].gl_Position;\
vec4 A = p0 - 2 * p1 + p2, B = 2 * (p1 - p0), C = p0;\
)"

constexpr const char *SHADER_GEOM_CURVE = R"(
#version 330

in VERTEX_SHADER_OUT {{
	int id;
}} vs_out[];
layout(triangles) in;
layout({}, max_vertices = {}) out;

uniform int u_count;
uniform int u_sample_num;
uniform mat4 u_transform;

void main() {{)"
										  GET_POINTS
										  R"(
	for (int i = 0; i < u_sample_num; ++i) {{
		float t = float(i) / (u_sample_num - 1);
		gl_Position = u_transform * ((A * t + B) * t + C);
		EmitVertex();
	}}
	EndPrimitive();
}}
)";

constexpr const char *SHADER_GEOM_CURVE_FILLED = R"(
#version 330

in VERTEX_SHADER_OUT {{
	int id;
}} vs_out[];
layout(triangles) in;
layout(triangle_strip, max_vertices = {}) out;

uniform int u_count;
uniform int u_sample_num;
uniform mat4 u_transform;

void main() {{)"
												 GET_POINTS
												 R"(
	vec4 last_point = C;
	for (int i = 1; i < u_sample_num; ++i) {{
		float t = float(i) / (u_sample_num - 1);
		vec4 now_point = (A * t + B) * t + C;
		gl_Position = u_transform * vec4(last_point.x, -1, 0, 1); EmitVertex();
		gl_Position = u_transform * vec4(now_point.x, -1, 0, 1); EmitVertex();
		gl_Position = u_transform * last_point; EmitVertex();
		EndPrimitive();
		gl_Position = u_transform * vec4(now_point.x, -1, 0, 1); EmitVertex();
		gl_Position = u_transform * last_point; EmitVertex();
		gl_Position = u_transform * now_point; EmitVertex();
		EndPrimitive(); last_point = now_point;
	}}
}}
)";

#define GET_POINTS_CURVE R"(\
vec4 p0 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.5);\
vec4 p1 = gl_in[1 - (vs_out[2].id & 1)].gl_Position;\
vec4 p2 = mix(p1, gl_in[2].gl_Position, 0.5);\
vec4 A = p0 - 2 * p1 + p2, B = 2 * (p1 - p0), C = p0;\
)"

constexpr const char *SHADER_GEOM_CIRCULAR_CURVE = R"(
#version 330

in VERTEX_SHADER_OUT {{
	int id;
}} vs_out[];
layout(triangles) in;
layout({}, max_vertices = {}) out;

uniform int u_count;
uniform int u_sample_num;
uniform mat4 u_transform;

void main() {{)"
												   GET_POINTS_CURVE
												   R"(
	for (int i = 0; i < u_sample_num; ++i) {{
		float t = float(i) / (u_sample_num - 1);
		gl_Position = u_transform * ((A * t + B) * t + C);
		EmitVertex();
	}}
	EndPrimitive();
}}
)";

constexpr const char *SHADER_GEOM_CIRCULAR_CURVE_FILLED = R"(
#version 330

in VERTEX_SHADER_OUT {{
	int id;
}} vs_out[];
layout(triangles) in;
layout(triangle_strip, max_vertices = {}) out;

uniform int u_count;
uniform int u_sample_num;
uniform mat4 u_transform;

void main() {{)"
														  GET_POINTS_CURVE
														  R"(
	vec4 last_point = C;
	for (int i = 1; i < u_sample_num; ++i) {{
		float t = float(i) / (u_sample_num - 1);
		vec4 now_point = (A * t + B) * t + C;
		gl_Position = u_transform * vec4(0, 0, 0, 1); EmitVertex();
		gl_Position = u_transform * last_point; EmitVertex();
		gl_Position = u_transform * now_point; EmitVertex();
		EndPrimitive(); last_point = now_point;
	}}
	EndPrimitive();
}}
)";

constexpr const char *SHADER_FRAG_CURVE = R"(
#version 330

out vec4 o_color;

uniform vec4 u_color;

void main() {
	o_color = u_color;
}
)";

constexpr const char *SHADER_FRAG_DOTS = R"(
#version 330

out vec4 o_color;

uniform vec4 u_color;

void main() {
	if (length(gl_PointCoord - vec2(0.5)) > 0.5) discard;
	o_color = u_color;
}
)";
