#pragma once

#include "../Rendering/renderer2d.h"
#include "../Rendering/Index_Buffer.h"

#include <array>

#include <memory>

struct Geometry_Vertex
{
	glm::vec2 position;
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f};
	float height = 1.0f;

	glm::vec2 get_tex_coord()
	{
		return texture_coord;
	}

	friend class Geometry;
private:
	glm::vec2 texture_coord = { 0,0 };
};


class Geometry
{
public:
	Geometry(std::vector<Geometry_Vertex>& vertices);
	Geometry(std::vector<Geometry_Vertex>& vertices, unsigned int* index, int num_indicies);
	~Geometry();

	void Add_index_buffer(unsigned int* index, int num_indicies);
	void render();
	void shutdown();

	void set_rotation(float rotation)
	{
		m_rotation = rotation;
	}
	float get_rotation()
	{
		return m_rotation;
	}

	void set_position(glm::vec2 pos)
	{
		m_position = glm::vec3(pos, 0.0f);
	}
	void set_position(glm::vec3 pos)
	{
		m_position = pos;
	}
	glm::vec3 get_position()
	{
		return m_position;
	}
	
	void set_scale(float scale)
	{
		m_scale = scale;
	}
	float get_scale()
	{
		return m_scale;
	}

	std::vector<Geometry_Vertex> get_vertices()
	{
		return m_vertices;
	}
	
	int get_num_vertices()
	{
		return m_num_vertex;
	}

	void set_layer(int layer)
	{
		m_layer = layer;
	}
	int get_layer()
	{
		return m_layer;
	}

	std::shared_ptr<Index_Buffer> get_index_buffer()
	{
		return m_ib;
	}


private:
	std::vector<Geometry_Vertex> m_vertices = {};
	std::shared_ptr<Index_Buffer> m_ib;

	
	int m_num_vertex;

	float m_rotation;
	glm::vec3 m_position;
	float m_scale;

	int m_layer = 0;

	bool m_shutdown;

private:
	void configure_texture_coords();
};

