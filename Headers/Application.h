#ifndef APPLICATION_H
#define APPLICATION_H
#include <iostream>
#include <vector>

class VAO;
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

    std::vector<VAO*> m_vaoList;
};

#endif