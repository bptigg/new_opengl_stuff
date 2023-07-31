#include "Sprite.h"

#include <sstream>

#define _USE_MATH_DEFINES 
#include <math.h>


static std::array<std::unique_ptr<Sprite>, static_cast<size_t>(Sprite::TextureID::NumberOfSprites) + 1> s_sprites;

Sprite::Sprite(TextureID sprite_id)
{
	s_sprites[(size_t)sprite_id] = std::make_unique<Sprite>(this);
	//m_Texture = GetSpriteTexture(sprite_id);

	if (int diff = (int)sprite_id - (int)TextureID::TileBasicIntCorner; diff <= 3)
	{
		m_BaseRotation = M_PI_2 * diff;
	}
}

Sprite::~Sprite()
{

}

void Sprite::RenderSprite(glm::vec2& position, glm::vec2& dimensions, float rotation, unsigned int render_layer, VertexType type, Sprite::TextureID id)
{
	if ((int)id < 0 || id == TextureID::NumberOfSprites)
	{
		std::stringstream ss;
		ss << "SPRITE ID: " << (int)id << "CAN'T BE CONVERTED TO A SPRITE" << std::endl;
		const char* message = ss.str().c_str();
		Log::warning(message);
	}

	size_t index = (size_t)id;
	s_sprites[index]->m_RenderSprite(position, dimensions, rotation, render_layer, type);
}

std::unique_ptr<Sprite>& Sprite::GetSprite(Sprite::TextureID SpriteId)
{
	if ((int)SpriteId < 0 || SpriteId == TextureID::NumberOfSprites)
	{
		std::stringstream ss;
		ss << "SPRITE ID: " << (int)SpriteId << "CAN'T BE CONVERTED TO A SPRITE" << std::endl;
		const char* message = ss.str().c_str();
		Log::warning(message);
	}

	size_t index = (size_t)SpriteId;
	return s_sprites[index];
}

void Sprite::m_RenderSprite(glm::vec2& position, glm::vec2& dimensions, float rotation, unsigned int render_layer, VertexType type)
{
	QUADrender_param sprite;
	if (type == VertexType::QUAD)
	{
		sprite.color = { 1.0f,1.0f,1.0f,1.0f };
		sprite.layer = render_layer;
		sprite.rotation = rotation;
		sprite.size = dimensions;
		sprite.Texture = m_Texture;

	}
	else if (type == VertexType::DEFUALT)
	{
		std::stringstream ss;
		ss << "NO VERTEX TYPE SPECIFIIED, RENDERING AS A QUAD: " << m_Texture << std::endl;
		const char* message = ss.str().c_str();

		Log::warning(message);

		m_RenderSprite(position, dimensions, rotation, render_layer, VertexType::QUAD);
	}

	renderer2d::draw_quad(position, sprite);
}

