#pragma once

#include "Shader.h"

#define _USE_MATH_DEFINES
#include <math.h>


class Light
{
public:
	enum class LIGHT_TYPE
	{
		POINT_LIGHT = 0,
		SPOTLIGHT,
		DEFAULT
	};

	Light(LIGHT_TYPE type, float intensity, glm::vec3 position, glm::vec3 color, glm::vec2 angle_range = { 0.0f, 2 * M_PI });
	
	void set_aspect_ratio(float width, float height);

	void send_to_shader(std::string shader, int index = 0);

private:
	float m_light_intesity = 0.0f;
	float m_light_height = 0.0f;
	LIGHT_TYPE m_type = LIGHT_TYPE::DEFAULT;

	glm::vec2 m_light_position = { 0.0f, 0.0f };
	glm::vec3 m_light_color = { 1.0f, 1.0f, 1.0f };

	glm::vec2 m_angle_range = { 0.0f, 2 * M_PI };
};

void update_screen_details(std::string shader);

