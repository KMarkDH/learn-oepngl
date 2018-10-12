#ifndef VERTEXARRAYBUFFER_H
#define VERTEXARRAYBUFFER_H

class VertexArrayBuffer
{
public:
    VertexArrayBuffer();
    ~VertexArrayBuffer();

    void bind() const;
    void unbind() const;
    void setLayout(unsigned int index, unsigned int size, unsigned int type, unsigned char normallized, int stride, const void* pointer);

private:

    unsigned int m_renderingId;
};

#endif