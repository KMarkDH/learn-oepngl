#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
public:

    IndexBuffer(const void* data, unsigned int datalen);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

private:

    unsigned int m_renderingId;
};

#endif