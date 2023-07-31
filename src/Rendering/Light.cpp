#include "Light.h"

#include "renderer2d.h"

static float s_screen_width = 1.0f;
static float s_screen_height = 1.0f;
static float s_aspect_ratio = 1.0f;
static bool s_shader_live = false;
static std::string s_lighting_shader = "";


Light::Light(LIGHT_TYPE type, float intensity, glm::vec3 position, glm::vec3 color, glm::vec2 angle_range)
	:m_type(type), m_light_intesity(2 * intensity), m_light_color(color), m_angle_range(angle_range)
{
	if (abs(position.x) > 1.0)
	{
		Log::variable<const float&>("position.x value non relative", position.x);
	}
	if(abs(position.y) > 1.0)
	{
		Log::variable<const float&>("position.y value non relative", position.y);
	}
	
	m_light_position = { position.x, position.y };
	m_light_height = position.z;
}

void Light::set_aspect_ratio(float width, float height)
{
	m_light_position = { (m_light_position.x / s_screen_width) * width, (m_light_position.y / s_screen_height) * height };

	s_screen_width = width;
	s_screen_height = height;
	float aspect = width / height;
	
	if (s_aspect_ratio != aspect)
	{
		s_shader_live = false;
		s_aspect_ratio = aspect;
	}

	if (s_shader_live == false)
	{
		update_screen_details(s_lighting_shader);
	}
}

void Light::send_to_shader(std::string shader, int index)
{
	auto shader_program = renderer2d::get_shader_library()->get(shader);

	std::stringstream ss;
	ss << "lights[" << index << "].";
	std::string base = ss.str();
	ss.str("");

	ss << base << "position";
	std::string uniform = ss.str();
	shader_program->set_uniform_3f(uniform, m_light_position.x, m_light_position.y, m_light_height);

	ss.str("");
	uniform.clear();
	ss << base << "color";
	uniform = ss.str();
	shader_program->set_uniform_3f(uniform, m_light_color.x, m_light_color.y, m_light_color.z);

	ss.str("");
	uniform.clear();
	ss << base << "intensity";
	uniform = ss.str();
	shader_program->set_uniform_1f(uniform, m_light_intesity);

	ss.str("");
	uniform.clear();
	ss << base << "type";
	uniform = ss.str();
	shader_program->set_uniform_1i(uniform, (int)m_type);

	ss.str("");
	uniform.clear();
	ss << base << "min_max_angle";
	uniform = ss.str();
	shader_program->set_uniform_2f(uniform, m_angle_range.x, m_angle_range.y);
}

void update_screen_details(std::string shader)
{
	s_shader_live = true;
	auto shader_program = renderer2d::get_shader_library()->get(shader);

	shader_program->set_uniform_1f("aspect_ratio", s_aspect_ratio);

	s_lighting_shader = shader;
}
