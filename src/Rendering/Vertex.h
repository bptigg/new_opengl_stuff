#pragma once

#include <glm/glm.hpp>

struct Character
{
	unsigned int tex_id = 0;
	glm::ivec2 size;
	glm::ivec2 bearing;
	uint16_t advance;
};

struct TextVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	
	float TexIndex;
};

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Colour;
	glm::vec2 TexCoord;

	float TexIndex;
	float TilingFactor;
};

struct CircleVertex
{
	glm::vec3 WorldPosition;
	glm::vec3 LocalPosition;
	glm::vec4 Colour;

	float thickness;
	float fade;
};

struct LineVertex
{
	glm::vec3 Position;
	glm::vec4 Colour;
};