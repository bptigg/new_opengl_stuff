#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;
uniform mat4 u_view_proj;

out vec2 v_tex_coord;

void main()
{
    v_tex_coord = a_tex_coord;
    gl_Position = u_view_proj * vec4(a_position, 1.0);
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

uniform sampler2D u_textures[32];

//uniform sampler2D gDepth;
uniform int gAlbedo;
//uniform int u_lights;

in vec2 v_tex_coord;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity; //0-1
};

//uniform Light lights[u_lights];

float inverse_sqaure_law(float intensity, float distance)
{
    float inverse_distance = 1.0 / distance;
    return intensity * (inverse_distance * inverse_distance);
};


void main()
{
    vec4 color = texture(u_textures[gAlbedo], v_tex_coord) * 0.1;
    o_color = vec4(color.x, color.y, color.z, 1.0);
}
