#include "CollisionBox.h"

CollisionBox::CollisionBox(glm::vec2 center, glm::vec2 dimensions)
{
	box_center = center;
	upper_bound = glm::vec2(dimensions.x / 2.0f, dimensions.y / 2.0f);
	lower_bound = glm::vec2(dimensions.x / -2.0f, dimensions.y / -2.0f);
}

bool CollisionBox::point_intersection(glm::vec2 point)
{
	float upper_x = box_center.x + upper_bound.x;
	float upper_y = box_center.y + upper_bound.y;
	float lower_x = box_center.x - lower_bound.x;
	float lower_y = box_center.y - lower_bound.y;

	if (point.x <= upper_x && point.x >= lower_x)
	{
		if (point.y <= upper_y && point.y >= lower_y)
		{
			return true;
		}
	}

	return false;
}

bool CollisionBox::CollisionBox_intersection(CollisionBox box2)
{
	glm::vec2 dimensions[2];
	dimensions[0] = { upper_bound.x - lower_bound.x, upper_bound.y - lower_bound.y };
	dimensions[1] = { box2.upper_bound.x - box2.lower_bound.x, box2.upper_bound.y - box2.lower_bound.y };

	glm::vec2 dist = box2.box_center - box_center;

	if (dist.x <= ((dimensions[0].x + dimensions[1].x) * 0.5f))
	{
		if (dist.y <= ((dimensions[0].y + dimensions[1].y) * 0.5f))
		{
			return true;
		}
	}

	return false;
}

void CollisionBox::update_bounds(glm::vec2 dimensions)
{
	upper_bound = glm::vec2(dimensions.x / 2.0f, dimensions.y / 2.0f);
	lower_bound = glm::vec2(dimensions.x / -2.0f, dimensions.y / -2.0f);
}
