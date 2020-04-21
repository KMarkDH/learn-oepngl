#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#define ASSERT(_cond_, ...) \
    if (!_cond_) \
        printf(__VA_ARGS__); \
        printf("Assertion : (%s, %s)\n", __FILE__, __LINE__);

class Application
{
public:

    static std::string WorkPath;
    static const std::string getFilePath(std::string file);

public:

    Application() = delete;
    explicit Application(std::string name, std::string arg);

    ~Application();
    void mainLoop();

private:

    void procPath(std::string arg);
    bool init(std::string name);
    void processKeyboard();
    void processCursor(double xpos, double ypos);
    void proceeFrameResize(int width, int height);
    GLFWwindow* m_window;
};

#endif //APPLICATION_H