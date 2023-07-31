#pragma once

#include "../Rendering/renderer2d.h"
#include "../utilities/CollisionBox.h"

//extern class Room;
class Room;

class Tile
{
public:
	enum class TileType
	{
		EMPTY = 0, //Default
		SOLID, //Wall
		SPAWN //Enemy spawn point
	};

	friend class Room;

public:
	Tile();
	Tile(glm::vec2& location, std::shared_ptr<CollisionBox> collision = nullptr, TileType type = TileType::EMPTY, glm::vec2* size = nullptr);

	bool GetCollision(std::shared_ptr<CollisionBox>& box2);

	void AddCollision(std::shared_ptr<CollisionBox>& collision);
	void AddSize(glm::vec2 size); //add a size value to the tile, and also generates a collision box for it

protected:
	TileType m_TileType = TileType::EMPTY;
	glm::vec2 m_Location = { 0,0 }; //relative to the room

private:
	std::shared_ptr<CollisionBox> m_CollisionBox;
	glm::vec2 m_Size;

};