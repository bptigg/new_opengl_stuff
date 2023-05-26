#shader vertex
#version 460 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_entity_id;

out vec4 v_color;

uniform mat4 u_View_Proj;
uniform mat4 u_Transform;

void main()
{
	v_color = a_Color;
	gl_Position = u_View_Proj * u_Transform * vec4(a_position, 1.0, 1.0);
};


#shader fragment
#version 460 core

layout(location = 0) out vec4 o_Color;

uniform vec4 v_color;

void main()
{
	o_Color = v_color;
};