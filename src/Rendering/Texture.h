#pragma once

#include <string>
#include <unordered_map>

#include "glDebug.h"
#include <glm/glm.hpp>

struct Texture_Data
{
	GLuint texture_id = 0;
	glm::vec2 size = glm::vec2{ 1.0f };

	bool bound = false;
	uint32_t slot = 0;
	bool alive = true;
	bool multisampled = false;
	bool always_rebind = false;
};

class Texture
{
public:
	static const unsigned int MAX_TEXTURE_SLOTS = 32;

public:

	static Texture_Data Load_Texture(std::string path);
	static GLuint Create_Texture(unsigned int width, unsigned int height, unsigned int color, unsigned int& renderer_id);
	static GLuint Create_Texture(unsigned int width, unsigned height, unsigned char* buffer);
	
	static void delete_texture(unsigned int id);

	static void bind(GLuint id, unsigned int slot = 0, bool multisampled = false) ;
	static void unbind(unsigned int slot);

};


class Texture_Library
{
public:

	~Texture_Library();
	void shutdown();

	void Add(const std::string& name, std::shared_ptr<Texture_Data>& data);
	
	std::shared_ptr<Texture_Data> Load(std::string& name, std::string& filepath);
	std::shared_ptr<Texture_Data> get(const std::string& name);
	void update_texture(const std::string& name, std::shared_ptr<Texture_Data> texture);

	bool texture_exists(const std::string& name) const;
private:
	std::unordered_map<std::string, std::shared_ptr<Texture_Data>> m_textures;
};
