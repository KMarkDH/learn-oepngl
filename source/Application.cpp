#include <functional>

#include <Application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <Shader.h>
#include <sstream>

#include "Texture.h"

static std::stringstream mLog;
static std::stringstream mError;


struct DirectionLight
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 lightDirection;
};

void drawConsole()
{
    ImGui::Begin("console");
    {
        ImGui::TextColored
        (
            ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
            "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate
        );
        ImGui::Text(mLog.str().c_str());
        const ImVec4 errorColor(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(errorColor, mError.str().c_str());
    }
    
    ImGui::End();
}

static bool curso_enable = false;
glm::vec3 cameraPos(0.0f, 0.0f, -3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

glm::vec3 lightColor(1.0f, 0.0f, 0.0f);
glm::vec3 lightPos(0.0f, 0.0f, 3.0f);

static float ambientStrength = 1.0f;
static float diffuseStrength = 1.0f;
static float specularStrength = 1.0f;

struct Material
{
    int shinnese = 8;
};

static Material material; 
static bool lightRotate = false;

std::string Application::WorkPath;

Application::Application(std::string name, std::string arg)
{
    std::cout.rdbuf(mLog.rdbuf());
    std::cerr.rdbuf(mError.rdbuf());
    std::clog.rdbuf(mError.rdbuf());

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
    const char* glsl_version = "#version 150";
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

    //setup imgui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //init imgui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    return true;
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

    if (glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        curso_enable = true;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        curso_enable = false;
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
        lightRotate = true;

    if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
        lightRotate = false;
}

double lastX, lastY;
double scale = 0.05f;
bool firstMove = true;;
double pitch = 0, yaw = 0;
void Application::processCursor(double xpos, double ypos)
{
    if (!curso_enable) return;
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

    std::string filePath = Application::getFilePath("/res/textures/container2.png");

    int width, height, channel;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    Texture container2("container2.png", 0, TextureType::CLAMP_EDGT_MIPMAP);
    Texture container2_specular("container2_specular.png", 1, TextureType::CLAMP_EDGT_MIPMAP);
    Texture lighting_maps_specular_color("lighting_maps_specular_color.png", 2, TextureType::CLAMP_EDGT_MIPMAP);
    Texture matrix("matrix.jpg", 3, TextureType::CLAMP_EDGT_MIPMAP);
    

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Shader shader("default-tmvp");
    glm::mat4 trans(1.0f);

    Shader sunShader("direction-light");
    // glm::mat4 sunTrans(1.0f);
    // sunTrans = glm::translate(sunTrans, lightPos);
    // sunTrans = glm::scale(sunTrans, glm::vec3(0.1f, 0.1f, 0.1f));

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

    

    Shader phoneShader("lights/multi-light-direction");


    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    DirectionLight directionLight;
    while (!glfwWindowShouldClose(m_window))
    {
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            drawConsole();

            ImGui::Begin("Material");
            ImGui::SliderInt("shinnese", &material.shinnese, 8, 1024);
            ImGui::End();

            ImGui::Begin("Direction-Light");
            ImGui::ColorEdit3("ambient", (float*)&directionLight.ambient);
            ImGui::ColorEdit3("diffuse", (float*)&directionLight.diffuse);
            ImGui::ColorEdit3("specular", (float*)&directionLight.specular);
            ImGui::InputFloat3("direction", (float*)&directionLight.lightDirection, 2);
            ImGui::End();
        }
        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        phoneShader.use();
        phoneShader.setMat4("uModel", trans);
        phoneShader.setMat4("uView", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        phoneShader.setMat4("uPerspective", glm::perspective(glm::radians(45.0f), (float)(640.0f / 360.0f), 0.1f, 100.0f));
        phoneShader.setVec3("viewPos", cameraPos);
        phoneShader.setInt("material.shinnese", material.shinnese);

        phoneShader.setInt("material.ambientTex", container2.getpointer());
        phoneShader.setInt("material.diffuseTex", container2.getpointer());
        phoneShader.setInt("material.specularTex", container2.getpointer());

        phoneShader.setVec3("directionLight.ambient", directionLight.ambient);
        phoneShader.setVec3("directionLight.diffuse", directionLight.diffuse);
        phoneShader.setVec3("directionLight.specular", directionLight.specular);
        phoneShader.setVec3("directionLight.lightDirection", directionLight.lightDirection);
        
        container2.bind();
        container2_specular.bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 sunTrans(1.0f);
        if (lightRotate)
        {
            lightPos.x = sinf((float)glfwGetTime()) * 3.0f;
            lightPos.z = cosf((float)glfwGetTime()) * 3.0f;
        }
        else
        {
            lightPos.x = sinf(glm::radians(45.0f)) * 3.0f;
            lightPos.z = cosf(glm::radians(45.0f)) * 3.0f;
        }

        sunTrans = glm::translate(sunTrans, lightPos);
        sunTrans = glm::scale(sunTrans, glm::vec3(0.1f, 0.1f, 0.1f));

        sunShader.use();
        sunShader.setInt("texturePtr", 0);
        sunShader.setMat4("model", sunTrans);
        sunShader.setMat4("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        sunShader.setMat4("perspective", glm::perspective(glm::radians(45.0f), (float)(640.0f / 360.0f), 0.1f, 100.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTex);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
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