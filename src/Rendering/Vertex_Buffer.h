#pragma once
#include <stdint.h>

class Vertex_Buffer
{
private:
	uint32_t m_renderer_id;
	bool is_dynamic;
	int m_offset = 0;
	uint32_t m_buffer_size;
public:
	Vertex_Buffer(const void* data, uint32_t size);
	Vertex_Buffer(uint32_t size);
	~Vertex_Buffer();

	void bind() const;
	void unbind() const;

	void clear_buffer();
	bool add_to_buffer(const void* vertices, uint32_t size);

	inline unsigned int get_buffer_size() { return m_buffer_size; }
	inline bool can_store(uint32_t size) { return m_offset + size <= m_buffer_size; }
	inline bool is_empty() { return m_offset == 0; }
};

