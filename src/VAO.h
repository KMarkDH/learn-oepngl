// #ifndef VAO_H
// #define VAO_H
#include <iostream>

class VAO
{
public:

    VAO();

    ~VAO();

    void setVbo(const void*data, unsigned int datasize, unsigned int usage);
    void setIbo(const void*data, unsigned int datasize, unsigned int usage);
    void setPointer(unsigned int index, unsigned int size, unsigned int type, unsigned char normallized, int stride, const void* pointer);
    void bind();
    void unbind();

    void bindVbo();
    void unbindVbo();

    void bindIbo();
    void unbindIbo();

private:

    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;
};

// #endif