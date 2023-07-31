#pragma once
class Index_Buffer
{
private:
	unsigned int m_renderer_id;
	unsigned int m_count;
	bool ib_shutdown;
	bool m_custom = false;
public:
	Index_Buffer(const unsigned int* data, unsigned int count);
	Index_Buffer(unsigned int count);
	~Index_Buffer();

	void shutdown();
	void update_index_buffer(const unsigned int* data, unsigned int count);

	inline unsigned int get_count() const { return m_count; }

	void bind() const;
	void unbind() const;

	void set_custom(bool custom);
	bool get_custom();
};

