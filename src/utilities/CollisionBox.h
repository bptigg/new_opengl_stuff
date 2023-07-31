#pragma once

#include "glm/glm.hpp"

class CollisionBox
{
public:
	glm::vec2 upper_bound;
	glm::vec2 lower_bound;
	glm::vec2 box_center;
public:
	CollisionBox(glm::vec2 center, glm::vec2 dimensions);

	virtual bool point_intersection(glm::vec2 point);
	virtual bool CollisionBox_intersection(CollisionBox box2);

	void update_bounds(glm::vec2 dimensions);
};