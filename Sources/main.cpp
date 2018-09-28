#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void processInput(GLFWwindow*);
static void framebuffer_size_callback(GLFWwindow*, int, int);
static int CreateShader(const std::string&, const std::string&);
static unsigned int CompileShader(unsigned int, const std::string&);

int main(int argc, char * argv[])
{
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwSwapInterval(1.0);
    auto mWindow = glfwCreateWindow(480, 360, "CD_Waluas", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return -1;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    float positions[] = {
        0.5, 0.5, 0.4, 0.4, 0.2, //0
        0.5, -0.5,0.4, -0.4, 0.2,//1
        -0.5, -0.5,-0.4, -0.4, 0.2,//2
        -0.5, 0.5,-0.4, 0.4, 0.2//3
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,sizeof(float) * 5, (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,sizeof(float) * 5, (void*)(sizeof(float) * 2));
    
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    unsigned int indeces[] = {
        0,1,2,
        2,3,0
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indeces, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    std::string vShder =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) in vec4 aPos;\n"
    "layout (location = 1) in vec4 bPos;\n"
    "out vec4 g_color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = aPos;\n"
    "   g_color = vec4(bPos.xyz, 1.0);\n"
    "}\n";

    std::string fShder =
    "#version 330 core\n"
    "\n"
    "in vec4 g_color;\n"
    "layout(location = 0) out vec4 aColor;\n"
    "uniform vec4 u_Color;\n"
    "\n"
    "void main()\n"
    "{\n"
    "aColor = vec4(g_color.xyz, 1.0);\n"
    "}\n";

    int program = CreateShader(vShder, fShder);
    glUseProgram(program);
    int uniform = glGetUniformLocation(program, "u_Color");

    float r = 0.0f;
    float acceleration = 0.05;
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glUniform4f(uniform, r, 0.2f, 0.8f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//        glDrawArrays(GL_TRIANGLES, 0 , 6);
        glBindVertexArray(0);
        
        if ((r > 1.0f) || (r < 0.0f))
            acceleration = -acceleration;
        
        r += acceleration;

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Compile Error:" << message << std::endl;
    }
    return id;
}

    
int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    return program;
}