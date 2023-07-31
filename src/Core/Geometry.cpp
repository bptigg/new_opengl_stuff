#include "Geometry.h"

#include "../utilities/utility.h"

#include <functional>


Geometry::Geometry(std::vector<Geometry_Vertex>& vertices)
{
	m_vertices = {};
	for (int i = 0; i < vertices.size(); i++)
	{
		m_vertices.push_back(vertices[i]);
	}
	m_num_vertex = vertices.size();
	int num_triangles = (vertices.size() - 2);
	int num_indicies = num_triangles * 3;

	unsigned int* index = (unsigned int*)malloc(num_indicies * sizeof(unsigned int));
	unsigned int offset = 0;
	for (int i = 0; i < num_triangles; i++)
	{
		index[i + offset]			= 0;
		index[i + 1 + offset]		= 1 + i;
		index[i + 2 + offset]		= 2 + i;
		offset += 2;
	}

	m_ib = std::shared_ptr<Index_Buffer>(new Index_Buffer(index, num_indicies));
	free(index);
	m_ib->set_custom(false);

	configure_texture_coords();


	m_shutdown = false;
}

Geometry::Geometry(std::vector<Geometry_Vertex>& vertices, unsigned int* index, int num_indicies)
{
	m_vertices = {};
	for (int i = 0; i < vertices.size(); i++)
	{
		m_vertices.push_back(vertices[i]);
	}
	m_num_vertex = vertices.size();

	Add_index_buffer(index, num_indicies);

	m_shutdown = false;
}

Geometry::~Geometry()
{
	//free(m_vertices);
	if (!m_shutdown)
	{
		shutdown();
	}
}

void Geometry::shutdown()
{
	m_ib->shutdown();
	m_shutdown = true;
}

void Geometry::Add_index_buffer(unsigned int* index, int num_indicies)
{
	if (m_ib != nullptr)
	{
		m_ib->update_index_buffer(index, num_indicies);
	}
	else
	{
		m_ib = std::shared_ptr<Index_Buffer>(new Index_Buffer(index, num_indicies));
	}

	m_ib->set_custom(true);
}

void Geometry::render()
{
	renderer2d::draw_polygon(*this);
}

void Geometry::configure_texture_coords()
{
	auto temp = m_vertices;

	auto right = [](Geometry_Vertex vertex_a, Geometry_Vertex vertex_b)
	{
		bool more_right = vertex_a.position.x > vertex_b.position.x;

		return more_right;
	};

	auto left = [](Geometry_Vertex vertex_a, Geometry_Vertex vertex_b)
	{
		bool more_left = vertex_a.position.x < vertex_b.position.x;

		return more_left;
	};

	auto bottom = [](Geometry_Vertex vertex_a, Geometry_Vertex vertex_b)
	{
		bool lower = vertex_a.position.y < vertex_b.position.y;

		return lower;
	};

	auto top = [](Geometry_Vertex vertex_a, Geometry_Vertex vertex_b)
	{
		bool top = vertex_a.position.y > vertex_b.position.y;

		return top;
	};
	
	std::sort(temp.begin(), temp.end(), top);
	auto t = temp[0].position.y;
	
	std::sort(temp.begin(), temp.end(), bottom);
	auto b = temp[0].position.y;

	std::sort(temp.begin(), temp.end(), right);
	auto r = temp[0].position.x;

	std::sort(temp.begin(), temp.end(), left);
	auto l = temp[0].position.x;

	for (int i = 0; i < m_num_vertex; i++)
	{
		float x = Utility::normalize<float>(m_vertices[i].position.x, l, r);
		float y = Utility::normalize<float>(m_vertices[i].position.y, b, t);
		m_vertices[i].texture_coord = { x,y };
	}

}
