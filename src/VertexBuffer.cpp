#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int datalen)
:m_renderingId(0)
{
    glGenBuffers(1, &m_renderingId);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderingId);

    glBufferData(GL_ARRAY_BUFFER, datalen, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    
}

void VertexBuffer::bind()  const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderingId);
}

void VertexBuffer::unbind()  const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getBuffer() const
{
    return m_renderingId;
}