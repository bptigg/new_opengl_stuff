#pragma once

#include "Vertex_Buffer.h"
#include "Vertex_Buffer_Layout.h"
#include "Index_Buffer.h"

#include <memory>

class Vertex_Array
{
private:
	uint32_t m_renderer_id;
	std::shared_ptr<Index_Buffer> m_ib;
	bool va_shutdown;
public:
	Vertex_Array();
	~Vertex_Array();

	void shutdown();

	void add_buffer(const Vertex_Buffer& vb, const Vertex_Buffer_Layout& layout);
	void add_index_buffer(const std::shared_ptr<Index_Buffer>& ib);
	const std::shared_ptr<Index_Buffer>& get_ib();

	void bind() const;
	void unbind() const;
};

