#include "VertexArrayBuffer.h"
#include <glad/glad.h>

VertexArrayBuffer::VertexArrayBuffer()
:m_renderingId(0)
{
    glGenVertexArrays(1, &m_renderingId);
    glBindVertexArray(m_renderingId);
}

VertexArrayBuffer::~VertexArrayBuffer()
{

}

void VertexArrayBuffer::setLayout(unsigned int index, unsigned int size, unsigned int type, unsigned char normallized, int stride, const void* pointer)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normallized, stride, pointer);
}

void VertexArrayBuffer::bind() const
{
    glBindVertexArray(m_renderingId);
}

void VertexArrayBuffer::unbind() const
{
    glBindVertexArray(0);
}