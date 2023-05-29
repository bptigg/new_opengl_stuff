#include "Vertex_Array.h"
#include "glDebug.h"

Vertex_Array::Vertex_Array()
{
	GlCall(glGenVertexArrays(1, &m_renderer_id));
}

Vertex_Array::~Vertex_Array()
{
	if (!va_shutdown) { GlCall(glDeleteVertexArrays(1, &m_renderer_id)) };
}

void Vertex_Array::shutdown()
{
	va_shutdown = true;
	GlCall(glDeleteVertexArrays(1, &m_renderer_id));
}

void Vertex_Array::add_buffer(const Vertex_Buffer& vb, const Vertex_Buffer_Layout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.get_elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset));
		offset += element.count * Buffer_Element::size_of_type(element.type);
	}
	vb.unbind();
	unbind();
}

void Vertex_Array::add_index_buffer(const std::shared_ptr<Index_Buffer>& ib)
{
	bind();
	ib->bind();
	m_ib = ib;
	unbind();
}

void Vertex_Array::bind() const
{
	GlCall(glBindVertexArray(m_renderer_id));
}

void Vertex_Array::unbind() const
{
	GlCall(glBindVertexArray(0));
}
