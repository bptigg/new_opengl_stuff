#pragma once

#include <vector>
#include <GL/glew.h>

#include "glDebug.h"

struct Buffer_Element
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int size_of_type(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class Vertex_Buffer_Layout
{
private:
	std::vector<Buffer_Element> m_elements;
	unsigned int m_stride;
public:
	Vertex_Buffer_Layout()
		:m_stride(0)
	{

	}

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, count, GL_FALSE });
		m_stride += count * Buffer_Element::size_of_type(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * Buffer_Element::size_of_type(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * Buffer_Element::size_of_type(GL_UNSIGNED_BYTE);
	}

	inline unsigned int get_stride() const { return m_stride; }
	inline const std::vector<Buffer_Element>& get_elements() const { return m_elements; }
};

//change this file up to fix the static assert failure 