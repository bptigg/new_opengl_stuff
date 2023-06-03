#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_tex_coord;
layout(location = 3) in float a_tex_index;

uniform mat4 u_view_proj;

out vec4 v_color;
out vec2 v_tex_coord;
out float v_tex_index;

void main()
{
	v_color = a_color;
	v_tex_coord = a_tex_coord;
	v_tex_index = a_tex_index;

	gl_Position = u_view_proj * vec4(a_position,1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

in vec4 v_color;
in vec2 v_tex_coord;
in float v_tex_index;

uniform sampler2D u_textures[32];

void main()
{
	int index = int(v_tex_index);
	o_color = texture(u_textures[index], v_tex_coord) * v_color;
}