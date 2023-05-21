#include "Index_Buffer.h"

#include "glDebug.h"

Index_Buffer::Index_Buffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

Index_Buffer::Index_Buffer(unsigned int count)
    :m_count(count)
{
    unsigned int* data = new unsigned int[count];

    unsigned int offset = 0;
    for (unsigned int i = 0; i < count; i += 6)
    {
        data[i + 0] = 0 + offset;
        data[i + 1] = 1 + offset;
        data[i + 2] = 2 + offset;
        
        data[i + 3] = 2 + offset;
        data[i + 4] = 3 + offset;
        data[i + 5] = 0 + offset;

        offset += 4; 
    }

    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW));

    delete[] data;
}

Index_Buffer::~Index_Buffer()
{
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void Index_Buffer::bind() const 
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void Index_Buffer::unbind() const 
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
