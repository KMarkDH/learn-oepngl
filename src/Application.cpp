#include "Application.h"

#include "OGL.h"
#include "ShaderFile.h"
// #include "VAO.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayBuffer.h"
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Application::Application()
    : m_ticker(0.0f)
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
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        onFrame();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
}

unsigned int texture, texture1;

void Application::onInit()
{
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
    // float positions[] = {
    //     0.5, 0.5, 0.4, 0.4, 0.2, 1.0, 1.0, //0
    //     0.5, -0.5, 0.4, -0.4, 0.2, 1.0, 0, //1
    //     -0.5, -0.5, -0.4, -0.4, 0.2, 0, 0, //2
    //     -0.5, 0.5, -0.4, 0.4, 0.2, 0, 1    //3
    // };

    unsigned int indeces[] = {
        0, 1, 2,
        2, 3, 0};

    VertexArrayBuffer *vao = new VertexArrayBuffer();
    VertexBuffer *vbo = new VertexBuffer(vertices, sizeof(vertices));

    vao->setLayout(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    vao->setLayout(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
    //vao->setLayout(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 5));

    IndexBuffer *ibo = nullptr;//new IndexBuffer(indeces, sizeof(unsigned int) * 6);

    ShaderFile sf("res/basic.sha");
    std::string vShder = sf.getShader(ShaderFile::Type::Vertex_Shader);
    std::string fShder = sf.getShader(ShaderFile::Type::Fragment_Shader);

    Shader *shader = new Shader();
    shader->createShader(vShder, fShder);

    DrawUnit *du = new DrawUnit();
    du->vao = vao;
    du->ibo = ibo;
    du->shader = shader;

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *image_data = stbi_load("res/walrus.jpg", &width, &height, &nrChannels, 0);
    if (image_data)
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image_data);
    }

    int width1, height1, nrChannels1;
    unsigned char *image_data1 = stbi_load("res/grape.png", &width1, &height1, &nrChannels1, 0);
    if (image_data1)
    {
        glGenTextures(1, &texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data1);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image_data1);
    }

    // glUniform1i(glGetUniformLocation(shader->getShader(), "tex_1"), 0);
    // glUniform1i(glGetUniformLocation(shader->getShader(), "tex_2"), 1);
    m_vaoList.push_back(du);
}

void Application::onFrame()
{
    m_ticker += 1.0f / 60.0f;
    for (auto iter = m_vaoList.begin(); iter != m_vaoList.end(); ++iter)
    {
        DrawUnit *dUit = *iter;

        glUseProgram(dUit->shader->getShader());
        int uniform = glGetUniformLocation(dUit->shader->getShader(), "u_Color");
        glUniform4f(uniform, 0.4, 0.4, 0.2, 1.0);

        glUniform1i(glGetUniformLocation(dUit->shader->getShader(), "tex_1"), 0);
        glUniform1i(glGetUniformLocation(dUit->shader->getShader(), "tex_2"), 1);

        glm::mat4 trans;
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
        //trans = glm::translate(trans, glm::vec3(-0.2, -0.2, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(dUit->shader->getShader(), "transform"), 1, GL_FALSE, glm::value_ptr(trans));

        // glm::mat4 model;
        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // glm::mat4 view;
        // // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        // glm::mat4 projection;
        // projection = glm::perspective(glm::radians(45.0f), 480.0f / 360.0f, 0.1f, 100.0f);

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)480 / (float)360, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(dUit->shader->getShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(dUit->shader->getShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(dUit->shader->getShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // dUit->ibo->bind();
        dUit->vao->bind();

        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}