#pragma once

#include "../renderer2d.h"
#include "../Vertex.h"

#include <array>

#define SPRITE_ROT_90 1
#define SPRITE_ROT_180 2
#define SPRITE_ROT_270 3

#define SPRITE_FLIP_VERT = 1
#define SPRITE_FLIP_HOZ = 2

class Sprite
{
public:
	enum class TextureID
	{
		ErrorID = -1,

		TileBasicWall,
		TileBasicFloor,
		TileBasicIntCorner,
		TileBasicIntCorner90 = TileBasicIntCorner + SPRITE_ROT_90,
		TileBasicIntCornerRot180 = TileBasicIntCorner + SPRITE_ROT_180,
		TileBasicIntCornerRot270 = TileBasicIntCorner + SPRITE_ROT_270,
		TileBasicLight,

		NumberOfSprites
	};

public:
	Sprite(TextureID sprite_id);
	~Sprite();

	static void RenderSprite(glm::vec2& position, glm::vec2& dimensions, float rotation, unsigned int render_layer, VertexType type, TextureID id);
	static std::unique_ptr<Sprite>& GetSprite(TextureID SpriteId);

private:
	void m_RenderSprite(glm::vec2& position, glm::vec2& dimensions, float rotation, unsigned int render_layer, VertexType type);
private:
	std::string m_Texture;
	float m_BaseRotation = 0.0f;
};