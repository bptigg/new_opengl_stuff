#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;
uniform mat4 u_view_proj;

out vec2 v_tex_coord;

void main()
{
    gl_Position = u_view_proj * vec4(a_position, 1.0);
    v_tex_coord = a_tex_coord;
}

#shader fragment
#version 460 core

layout(location = 0) out vec4 o_color;

//uniform sampler2D gDepth;
uniform sampler2D gAlbedo;
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
    ivec2 fragpos = ivec2(gl_FragCoord.x, gl_FragCoord.y);
    //vec4 color = texelFetch(gDepth, fragpos, 0);

    vec4 Albedo = texture(gAlbedo, v_tex_coord); //* 0.8;
    //vec3 lighting = vec3(Albedo.x, Albedo.y, Albedo.z);

    //for (int i = 0; i < u_lights; i++)
    //{
    //    if(color.x > lights[i].position.z)
    //    {
    //        continue;
    //    }
    //    float distance = 3dpythag(fragpos.x, fragpos.y, color.x);
    //    float intensity = inverse_sqaure_law(lights[i].intensity, distance);
    //    lighting = lighting + (Albedo * intensity);
    //}

    //if(color.x > 0.5)
    //{
    //    o_color = vec4(lighting, 1.0);
    //    return;
    //}

    //float d_x = fragpos.x - 640.0;
    //float d_y = fragpos.y - 360.0;
    //float d_z = color.x - 0.5;
//
    //float distance = sqrt(d_x*d_x + d_y*d_y + d_z*d_z);
    ////float intensity = inverse_sqaure_law(lights[i].intensity, distance);
    //float intensity = inverse_sqaure_law(0.8, distance);
    //lighting = lighting + intensity;

    //o_color = vec4(lighting,1.0);
    o_color = Albedo;
}
