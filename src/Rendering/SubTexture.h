#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

class SubTexture
{
public:
	SubTexture(const std::string& parent_texture, const glm::vec2& min, const glm::vec2& max, bool TrueSubTexture = false);

	const std::string& get_parent_texture() const { return m_parent; }
	const glm::vec2* get_tex_coords() const { return m_tex_coords; }

	const bool GetTrueSubTex() const { return m_TrueSubTex;  }

	static std::shared_ptr<SubTexture> create_from_coords(const std::string& parent_name, const Texture_Data& parent, const glm::vec2 coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize = {1,1});

private:
	std::string m_parent;
	bool m_TrueSubTex;
	glm::vec2 m_tex_coords[4];

};

class SubTexture_Library
{
public:

	~SubTexture_Library();

	void Add(const std::string& name, const std::shared_ptr<SubTexture>& sub_texture);
	void Add(const std::shared_ptr<SubTexture>& false_sub_texture); //used for false sub textures where the the sub texture is the og texture

	std::shared_ptr<SubTexture> create(const std::string& name, const std::string& parent_name, const Texture_Data& parent, const glm::vec2 coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize = { 1,1 });
	std::shared_ptr<SubTexture> create_false(const std::string& name, const std::string& parent_name, const Texture_Data& parent);
	std::shared_ptr<SubTexture> get(const std::string& name);

	bool sub_texture_exists(const std::string& name) const;
private:
	std::unordered_map<std::string, std::shared_ptr<SubTexture>> m_SubTextures;
};