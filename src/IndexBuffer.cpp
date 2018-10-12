#include "IndexBuffer.h"
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const void* data, unsigned int datalen)
:m_renderingId(0)
{
    glGenBuffers(1, &m_renderingId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderingId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, datalen, data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{

}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderingId);
}

void IndexBuffer::unbind()  const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}