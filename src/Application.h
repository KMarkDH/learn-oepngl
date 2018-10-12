#ifndef APPLICATION_H
#define APPLICATION_H
#include <iostream>
#include <vector>

class VertexArrayBuffer;
class IndexBuffer;
class Shader;
struct DrawUnit {
    VertexArrayBuffer* vao;
    IndexBuffer* ibo;
    Shader* shader;
};

class Application
{
public:

    Application();
    ~Application();
    void init();
    bool exit();
    

private:

    void onInit();
    void onFrame();

private:

    float m_ticker;
    std::vector<DrawUnit*> m_vaoList;


};

#endif