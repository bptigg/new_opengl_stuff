#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"


Texture_Data Texture::Load_Texture(std::string path)
{
	int width, height, BPP;
	unsigned int renderer_id;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* local_buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

	GlCall(glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GlCall(glGenerateTextureMipmap(renderer_id));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (local_buffer != nullptr)
	{
		stbi_image_free(local_buffer);
	}

	Log::variable<const std::string&>("Texture initialised", path);
	return { renderer_id, {width, height}, false, true };
}

GLuint Texture::Create_Texture(unsigned int width, unsigned int height, unsigned int color, unsigned int& renderer_id)
{
	GlCall(glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));


	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

	//GlCall(glGenerateTextureMipmap(renderer_id));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	return renderer_id;
}

GLuint Texture::Create_Texture(unsigned int width, unsigned height, unsigned char* buffer)
{
	GLuint renderer_id = 0;
	GlCall(glGenTextures(1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (width != 0 && height != 0)
	{
		//GlCall(glGenerateTextureMipmap(renderer_id));
	}

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	return renderer_id;
}

void Texture::delete_texture(unsigned int id)
{
	GlCall(glDeleteTextures(1, &id));
}

void Texture::bind(GLuint id,unsigned int slot, bool multisampled) 
{
	if (slot > MAX_TEXTURE_SLOTS - 1)
	{
		Log::crit("Trying to bind more than 32 textures!", __FILE__, __LINE__);
	}

	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	if (!multisampled)
	{
		GlCall(glBindTexture(GL_TEXTURE_2D, id));
	}
	else
	{
		GlCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id));
	}
}

void Texture::unbind(unsigned int slot)
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture_Library::~Texture_Library()
{
	for (auto it = m_textures.begin(); it != m_textures.end(); it++)
	{
		Texture::delete_texture(it->second->texture_id);
		it->second->alive = false;
		Log::variable<const std::string>("Texture deleted: ", it->first);
	}
	m_textures.clear();
}

void Texture_Library::Add(const std::string& name, std::shared_ptr<Texture_Data>& data)
{
	if (!texture_exists(name)) { Log::warning("Texture already exsits in library"); }
	m_textures[name] = data;
}

std::shared_ptr<Texture_Data> Texture_Library::Load(std::string& name, std::string& filepath)
{
	auto data = std::shared_ptr<Texture_Data>(new Texture_Data(Texture::Load_Texture(filepath)));
	Add(name, data);
	return data;
}

std::shared_ptr<Texture_Data> Texture_Library::get(const std::string& name)
{
	if (texture_exists(name)) {
		Log::crit("Texture not found", __FILE__, __LINE__);
	}

	return m_textures[name];
}

bool Texture_Library::texture_exists(const std::string& name) const
{
	return m_textures.find(name) == m_textures.end();
}


