#shader vertex
#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 2) in vec2 a_tex_coord;
uniform mat4 u_view_proj;

out vec2 v_tex_coord;
out vec3 v_position;
out mat4 v_view_proj;

void main()
{
    v_tex_coord = a_tex_coord;
    gl_Position = u_view_proj * vec4(a_position, 1.0);
    v_position = a_position;
    v_view_proj = u_view_proj;
}

#shader fragment
#version 460 core

const float M_PI = 3.14159265358979323846264338327950288;

layout(location = 0) out vec4 o_color;

uniform sampler2D u_textures[32];

uniform int gPosition;
uniform int gAlbedo;
uniform int u_lights;

in vec2 v_tex_coord;
in vec3 v_position;
in mat4 v_view_proj;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity; //0-1
    int type;
    vec2 min_max_angle;
};

uniform float aspect_ratio;
uniform Light lights[100];
uniform float zoom_level;


vec3 point_light(Light light, float distance, vec3 Albedo, float z)
{
    float new_intensisty = (light.intensity * z) - distance;
    if(new_intensisty < 0.0)
    {
        new_intensisty = 0.0;
    }

    return Albedo * new_intensisty * light.color;
}

vec3 spot_light(Light light, float distance, vec2 xy, vec3 Albedo, float z)
{
    float angle = 0;
    angle = atan(xy.y, xy.x) + M_PI;
    angle = angle / (2*M_PI);

    float max_angle = light.min_max_angle.x / (2*M_PI);
    float min_angle = light.min_max_angle.y / (2*M_PI);

    float new_intensisty = 0.0;
    
    if(angle > max_angle && angle < min_angle)
    {
         new_intensisty = 0.0;
    }
    else
    {
         new_intensisty = (light.intensity * z) - distance;
         {
             if(new_intensisty < 0.0)
             {
                 new_intensisty = 0.0;
             }
         }
    }

    return Albedo * new_intensisty * light.color;
}

void main()
{
    vec4 tex_color = texture(u_textures[gAlbedo], v_tex_coord);
    vec3 color = vec3(tex_color.x, tex_color.y, tex_color.z);
    vec3 lighting = color * 0.1;
    vec4 texture_position = texture(u_textures[gPosition], v_tex_coord);

    float x = 0.0;
    float y = 0.0;
    float dist = 0.0;

    float zoom = 1 / zoom_level;

    for (int i = 0; i < u_lights; i++)
    {
        vec4 position = vec4(lights[i].position, 1.0);
        vec4 lightPos = v_view_proj * position;
        
        if(texture_position.z >= lights[i].position.z)
        {
            lighting = lighting + 0.0f;
        }
        else
        {
            x = (lightPos.x - texture_position.x) * aspect_ratio;
            y = (lightPos.y - texture_position.y);
            dist = sqrt(x*x + y*y);

            if(lights[i].type == 0)
            {
                lighting = lighting + point_light(lights[i], dist, color, zoom);
            }
            else if(lights[i].type == 1)
            {
                lighting = lighting + spot_light(lights[i], dist, texture_position.xy, color, zoom);
            }

        }
    }

    o_color = vec4(lighting,1.0);
    //o_color = vec4(texture_position.z, test.position.z, 0.0,1.0);

};
