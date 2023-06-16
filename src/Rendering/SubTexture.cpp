#include "SubTexture.h"

SubTexture::SubTexture(const std::string& parent_texture, const glm::vec2& min, const glm::vec2& max, bool TrueSubTexture)
	:m_parent(parent_texture), m_TrueSubTex(TrueSubTexture)
{
	m_tex_coords[0] = { min.x, min.y };
	m_tex_coords[1] = { max.x, min.y };
	m_tex_coords[2] = { max.x, max.y };
	m_tex_coords[3] = { min.x, max.y };
}

std::shared_ptr<SubTexture> SubTexture::create_from_coords(const std::string& parent_name, const Texture_Data& parent, const glm::vec2 coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize)
{
	glm::vec2 min = { (coords.x * CellSize.x) / parent.size.x, (coords.y * CellSize.y) / parent.size.y };
	glm::vec2 max = { ((coords.x + SpriteSize.x) * CellSize.x) / parent.size.x, ((coords.y + SpriteSize.y) * CellSize.y) / parent.size.y };
	return std::shared_ptr<SubTexture>(new SubTexture(parent_name, min, max, true));
}

SubTexture_Library::~SubTexture_Library()
{
	m_SubTextures.clear();
}

void SubTexture_Library::Add(const std::string& name, const std::shared_ptr<SubTexture>& sub_texture)
{
	if (!sub_texture_exists(name)) { Log::crit("Texture already exsits in library", __FILE__, __LINE__); }
	m_SubTextures[name] = sub_texture;
}

void SubTexture_Library::Add(const std::shared_ptr<SubTexture>& false_sub_texture)
{
	auto& name = false_sub_texture->get_parent_texture();
	Add(name, false_sub_texture);
}

std::shared_ptr<SubTexture> SubTexture_Library::create(const std::string& name, const std::string& parent_name, const Texture_Data& parent, const glm::vec2 coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize)
{
	auto sub_texture = SubTexture::create_from_coords(parent_name, parent, coords, CellSize, SpriteSize);
	Add(name, sub_texture);
	return sub_texture;
}

std::shared_ptr<SubTexture> SubTexture_Library::create_false(const std::string& name, const std::string& parent_name, const Texture_Data& parent)
{
	auto sub_texture = SubTexture::create_from_coords(parent_name, parent, { 0,0 }, { parent.size.x, parent.size.y });
	Add(name, sub_texture);
	return sub_texture;
}

std::shared_ptr<SubTexture> SubTexture_Library::get(const std::string& name)
{
	if (sub_texture_exists(name)) { Log::crit("Texture not found", __FILE__, __LINE__); }
	return m_SubTextures[name];
}

bool SubTexture_Library::sub_texture_exists(const std::string& name) const
{
	return m_SubTextures.find(name) == m_SubTextures.end();
}
