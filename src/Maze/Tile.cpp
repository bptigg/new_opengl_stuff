#include "Tile.h"

#include "../utilities/KeyValues.h"

Tile::Tile()
{
	m_Location = { 0,0 };
	m_TileType = TileType::EMPTY;
	m_CollisionBox = std::shared_ptr<CollisionBox>(new CollisionBox(m_Location, { DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE }));
	m_Size = { DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE };
}

Tile::Tile(glm::vec2& location, std::shared_ptr<CollisionBox> Collision, TileType type, glm::vec2* size)
{
	m_Location = location;
	m_TileType = type;
	if (size == nullptr)
	{
		m_CollisionBox = std::shared_ptr<CollisionBox>(new CollisionBox(m_Location, { DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE }));
		m_Size = { DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE };
	}
	else
	{
		m_CollisionBox = std::shared_ptr<CollisionBox>(new CollisionBox(m_Location, *size));
		m_Size = *size;
	}
}

bool Tile::GetCollision(std::shared_ptr<CollisionBox>& box2)
{
	return m_CollisionBox->CollisionBox_intersection(*box2);
}

void Tile::AddCollision(std::shared_ptr<CollisionBox>& collision)
{
	m_CollisionBox = std::move(collision);
}

void Tile::AddSize(glm::vec2 size)
{
	m_Size = size;
	m_CollisionBox->update_bounds(size);
}
