#shader vertex
#version 460 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_color;

uniform mat4 u_view_proj;

layout(location = 0) out vec4 color;

void main()
{
    color = a_color;
    gl_Position = u_view_proj * vec4(a_pos, 1.0);
}

#shader fragment
#version 460 core
layout(location = 0) out vec4 o_color;

layout(location = 0) in vec4 color;

void main()
{
    o_color = color;
}