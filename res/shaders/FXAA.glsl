#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 3) in float a_tex_index;

uniform mat4 u_view_proj; //really temporary 

out float v_tex_index;

void main()
{
    v_tex_index = a_tex_index;
    gl_Position = u_view_proj * vec4(a_position, 1.0);
}

#shader fragment
#version 460 core

layout (location = 0) out vec4 o_color;

in float v_tex_index;

uniform sampler2DMS u_textures[32];

float rgb2luma(vec3 rgb)
{
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

void main()
{
    int tex_index = int(v_tex_index);
    
    //vec3 ColorCentre = texture(u_textures, gl_FragCoord.x, gl_FragCoord.y
}