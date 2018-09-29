#include "VAO.h"
#include "OGL.h"

VAO::VAO()
:m_vao(-1)
,m_vbo(-1)
,m_ibo(-1)
{
    glGenVertexArrays(1, &m_vao);
    std::cout << "CONSTRUCTION VAO" << std::endl;
}

 VAO::~VAO()
{
    unbind();
    m_vao = -1;
    m_vbo = -1;
    m_ibo = -1;
}

void VAO::setVbo(const void*data, unsigned int datasize, unsigned int usage)
{
    bind();
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, datasize, data, usage);

    unbindVbo();
    unbind();
}

 void VAO::setIbo(const void *data, unsigned int datasize, unsigned int usage)
{
    bind();
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize, data, usage);

    unbindIbo();
    unbind();
}


 void VAO::setPointer(unsigned int index, unsigned int size, unsigned int type, unsigned char normallized, int stride, const void* pointer)
{
    bind();
    bindVbo();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normallized, stride, pointer);

    unbindVbo();
    unbind();
}

 void VAO::bindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

 void VAO::unbindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

 void VAO::bindIbo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

 void VAO::unbindIbo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

 void VAO::bind()
{
    glBindVertexArray(m_vao);
}

 void VAO::unbind()
{
    glBindVertexArray(0);
}