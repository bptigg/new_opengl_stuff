#pragma once

#include <glm/glm.hpp>
#include <string>

enum class render_type
{
	QUAD = 0,
	CIRCLE,
	LINE,
	TEXT, //It exists but won't be implemented until the others are
	DEFAULT
};

struct render_object
{
	render_type type = render_type::DEFAULT;

	//All types
	glm::vec4 color = glm::vec4(1.0f);
	int layer = 0;

	//QUAD and CIRCLE
	glm::mat4 transform = glm::mat4(1.0f);

	//QUAD only
	std::string Texture = "";
	float tiling_factor = 1.0f;

	//CIRCLE only 
	float thickness = 1.0f;
	float fade = 0.005f;

	//Line only
	const glm::vec3 p0 = glm::vec3(0.0f);
	const glm::vec3 p1 = glm::vec3(0.0f);
};

class Rendering_manager
{
public:
	static void submit_queue_object(render_object& object);

};

