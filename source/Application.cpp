#include <functional>

#include <Application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Shader.h>

glm::vec3 cameraPos(0.0f, 0.0f, -3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightColor(1.0f, 0.0f, 0.0f);
glm::vec3 lightPos(0.0f, 0.0f, 3.0f);

std::string Application::WorkPath;

Application::Application(std::string name, std::string arg)
{
    procPath(arg);
    init(name);
}

void Application::procPath(std::string arg)
{
#if defined(_WIN32) || defined(WIN32)
    size_t dp = arg.rfind('\\');
#else
    size_t dp = arg.rfind('/');
#endif
    
    Application::WorkPath = arg.substr(0, dp);
}

bool Application::init(std::string name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__

    GLFWwindow* window = glfwCreateWindow(640, 360, name.c_str(), NULL, NULL);
    if (!window) return false;
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    auto frameCB = [](GLFWwindow* w, int width, int height) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->proceeFrameResize(width, height);
    };
    glfwSetFramebufferSizeCallback(window, frameCB);

    auto cursorCB = [](GLFWwindow* w, double x, double y) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->processCursor(x, y);
    };
    glfwSetCursorPosCallback(window, cursorCB);
    m_window = window;

    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(1);
    
    return true;
}

Application::~Application()
{
    glfwTerminate();
}

void Application::processKeyboard()
{
    //press escape to close window
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraFront * 0.1f;
    }

    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraFront * 0.1f;
    }

    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraUp, cameraFront)) * 0.1f;
    }

    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraUp, cameraFront)) * 0.1f;
    }
}

double lastX, lastY;
double scale = 0.05f;
bool firstMove = true;;
double pitch = 0, yaw = 0;
void Application::processCursor(double xpos, double ypos)
{
    if (firstMove)
    {
        lastX = xpos;
        lastY = ypos;
        firstMove = false;
    }

    double offsetX = xpos - lastX;
    double offsetY = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    pitch += offsetY * scale;
    yaw += offsetX * scale;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front(0.0f);
    front.x = cosf((float)glm::radians(pitch)) * cosf((float)glm::radians(yaw));
    front.y = sinf((float)glm::radians(pitch));
    front.z = cosf((float)glm::radians(pitch)) * sinf((float)glm::radians(yaw));
    cameraFront = glm::normalize(front);
}

void Application::proceeFrameResize(int width, int height)
{
    if (!m_window) return;
    glViewport(0, 0, width, height);
}

void Application::mainLoop()
{
    float vertices[] = 
    {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,      0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,      0.0f,  0.0f, -1.0f, 
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,      0.0f,  0.0f, -1.0f, 
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,      0.0f,  0.0f, -1.0f, 
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,      0.0f,  0.0f, -1.0f, 
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,      0.0f,  0.0f, -1.0f, 

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,      0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,      0.0f,  0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,      0.0f,  0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,      0.0f,  0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,      0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,      0.0f,  0.0f, 1.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,      -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,      -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      -1.0f,  0.0f,  0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      1.0f,  0.0f,  0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,      1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,      1.0f,  0.0f,  0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,      0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,      0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,      0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,      0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,      0.0f, -1.0f,  0.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,      0.0f,  1.0f,  0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,      0.0f,  1.0f,  0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,      0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,      0.0f,  1.0f,  0.0f
    };

    unsigned int VAO = 0, VBO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int texture;
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::vec4 boarderColor(0.0f, 0.0f, 0.0f, 0.0f);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(boarderColor));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    std::string filePath = Application::getFilePath("/res/textures/container.jpg");

    int width, height, channel;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader shader("default-tmvp");
    glm::mat4 trans(1.0f);

    Shader sunShader("direction-light");
    glm::mat4 sunTrans(1.0f);
    sunTrans = glm::translate(sunTrans, lightPos);
    sunTrans = glm::scale(sunTrans, glm::vec3(0.1f, 0.1f, 0.1f));

    unsigned int sunTex;
    glGenTextures(1, &sunTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    data = stbi_load(Application::getFilePath("/res/textures/sun.jpg").c_str(), &width, &height, &channel, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    Shader phoneShader("default-tmvp-phong");


    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    while (!glfwWindowShouldClose(m_window))
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        trans = glm::rotate(trans, glm::radians(((float)glfwGetTime()) * 0.04f), glm::vec3(0.0f, 1.0f, 0.0f));
        phoneShader.use();
        phoneShader.setInt("texturePtr", 0);
        phoneShader.setMat4("model", trans);
        phoneShader.setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        phoneShader.setMat4("perspective", glm::perspective(glm::radians(45.0f), (float)(640.0f / 360.0f), 0.1f, 100.0f));
        phoneShader.setVec3("lightColor", lightColor);
        phoneShader.setVec3("lightPos", lightPos);
        phoneShader.setVec3("viewPos", cameraPos);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        sunShader.use();
        sunShader.setInt("texturePtr", 0);
        sunShader.setMat4("model", sunTrans);
        sunShader.setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        sunShader.setMat4("perspective", glm::perspective(glm::radians(45.0f), (float)(640.0f / 360.0f), 0.1f, 100.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTex);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        this->processKeyboard();
    }
}

const std::string Application::getFilePath(std::string file)
{
    std::string ret = Application::WorkPath + file;
#if defined(_WIN32) || defined(WIN32)
    for (int i = 0; i < ret.size(); ++i)
    {
        if (ret[i] == 0x002F)
            ret[i] = 0x005C;
    }
#endif
    return ret;
}