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
uniform int u_samples;

void main()
{
    int tex_index = int(v_tex_index);
    ivec2 texture_position = ivec2(gl_FragCoord.x, gl_FragCoord.y);
    
    vec4 samples[16];
    for (int i = 0; i < u_samples; i++)
    {
        samples[i] = texelFetch(u_textures[tex_index], texture_position, i);
    }

    for (int i = 0; i < u_samples; i++)
    {
        o_color = o_color + samples[i];
    }
    o_color = o_color / float(u_samples);

    //vec4 sample1 = texelFetch(u_textures[tex_index], texture_position, 0);
    //vec4 sample2 = texelFetch(u_textures[tex_index], texture_position, 1);
    //vec4 sample3 = texelFetch(u_textures[tex_index], texture_position, 2);
    //vec4 sample4 = texelFetch(u_textures[tex_index], texture_position, 3);

    //o_color = (sample1 + sample2 + sample3 + sample4) / 4.0f;
}