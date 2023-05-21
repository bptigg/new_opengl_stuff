#pragma once
class Index_Buffer
{
private:
	unsigned int m_renderer_id;
	unsigned int m_count;
public:
	Index_Buffer(const unsigned int* data, unsigned int count);
	Index_Buffer(unsigned int count);
	~Index_Buffer();

	inline unsigned int get_count() const { return m_count; }

	void bind() const;
	void unbind() const;
};

