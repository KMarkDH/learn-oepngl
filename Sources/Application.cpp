#include "Application.h"

#include "OGL.h"
#include "ShaderFile.h"
#include "VAO.h"
#include "Shader.h"

Application::Application()
{
    
}

Application::~Application()
{

}

void Application::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwSwapInterval(1.0);
    auto mWindow = glfwCreateWindow(480, 360, "CD_Waluas", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr)
    {
        fprintf(stderr, "Failed to Create OpenGL Context");
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    onInit();

    while (glfwWindowShouldClose(mWindow) == false)
    {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        onFrame();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Application::onInit()
{
    float positions[] = {
        0.5, 0.5, 0.4, 0.4, 0.2, //0
        0.5, -0.5,0.4, -0.4, 0.2,//1
        -0.5, -0.5,-0.4, -0.4, 0.2,//2
        -0.5, 0.5,-0.4, 0.4, 0.2//3
    };

    unsigned int indeces[] = {
        0,1,2,
        2,3,0
    };

    VAO* vao = new VAO();
    vao->setVbo(positions, sizeof(float) * 20, GL_STATIC_DRAW);
    vao->setPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    vao->setPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
    vao->setIbo(indeces, 6 * sizeof(unsigned int), GL_STATIC_DRAW);
    vao->unbind();

    m_vaoList.push_back(vao);

    ShaderFile sf("res/basic.sha");
    std::string vShder = sf.getShader(ShaderFile::Type::Vertex_Shader);
    std::string fShder = sf.getShader(ShaderFile::Type::Fragment_Shader);

    Shader* shader = new Shader();
    int program = shader->createShader(vShder, fShder);
    glUseProgram(program);
}

void Application::onFrame()
{
    for(auto iter = m_vaoList.begin(); iter != m_vaoList.end(); ++iter)
    {
        VAO* vao = *iter;
        vao->bind();
        vao->bindIbo();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        vao->unbind();
    }
}