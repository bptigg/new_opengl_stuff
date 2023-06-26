#shader vertex
#version 460 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;
layout(location = 3) in float a_tex_index;

uniform mat4 u_view_proj;

struct Vertex_Output
{
	vec4 color;
	vec2 tex_coord;
	float tex_index;
};

layout(location = 0) out Vertex_Output Output;

void main()
{
	Output.color = a_color;
	Output.tex_coord = a_tex_coord;
	Output.tex_index = a_tex_index;

	gl_Position = u_view_proj * vec4(a_pos, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

struct Vertex_Output
{
	vec4 color;
	vec2 tex_coord;
	float tex_index;
};

layout(location = 0) in Vertex_Output Output;
uniform sampler2D u_textures[32];

void main()
{
	int index = int(Output.tex_index);
	o_color = vec4(Output.color.r, Output.color.g, Output.color.b, Output.color.a * texture(u_textures[index], Output.tex_coord).r);

}
