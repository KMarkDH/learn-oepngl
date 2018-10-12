#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer
{
public:

    VertexBuffer(const void* data, unsigned int datalen);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int getBuffer() const;

private:

    unsigned int m_renderingId;
};

#endif