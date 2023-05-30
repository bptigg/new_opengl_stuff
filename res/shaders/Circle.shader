#shader vertex
#version 460 core

layout(location = 0) in vec3 a_world_position;
layout(location = 1) in vec3 a_local_position;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;

uniform mat4 u_view_proj;

struct Vertex_Output
{
	vec3 Local_Position;
	vec4 color;
    float thickness;
    float fade;
};

layout(location = 0) out Vertex_Output Output;

void main()
{
    Output.Local_Position = a_local_position;
    Output.color = a_color;
    Output.thickness = a_thickness;
    Output.fade = a_fade;
    
    gl_Position = u_view_proj * vec4(a_world_position, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

struct Vertex_Output
{
	vec3 Local_Position;
	vec4 color;
    float thickness;
    float fade;
};

layout (location = 0) in Vertex_Output Input;

void main()
{
    float distance = 1.0 - length(Input.Local_Position);
    float circle = smoothstep(0.0, Input.fade, distance);
    circle *= smoothstep(Input.thickness + Input.fade, Input.thickness, distance);

    if(circle == 0.0)
    {
        discard;
    }

    o_color = Input.color;
    o_color.a *= circle;
}
