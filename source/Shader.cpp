#include <Shader.h>

#include <string.h>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string name)
{
    std::cout << Application::WorkPath + "\\res\\shaders\\" + name + ".shader" << std::endl;
    createShader(Application::WorkPath + "/res/shaders/" + name + ".shader");
}

Shader::~Shader()
{

}

void Shader::use()
{
    glUseProgram(m_shader);
}

void Shader::setInt(std::string uniform, int value)
{
    unsigned int u = glGetUniformLocation(m_shader, uniform.c_str());
    glUniform1i(u, value);
}

void Shader::setMat4(std::string uniform, glm::mat4 value)
{
    unsigned int u = glGetUniformLocation(m_shader, uniform.c_str());
    glUniformMatrix4fv(u, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::createShader(std::string fileName)
{
    std::fstream file(fileName, std::ios::in);

    enum class ShaderType {
        Vertex = 0,
        Fragment = 1
    };
    std::string ss[2];
    unsigned int index = 0;
    char buffer[512];
    while(!file.eof())
    {
        file.getline(buffer, 512);
        if (strstr(buffer, "@vertex") != nullptr)
        {
            index = (unsigned int)ShaderType::Vertex;
        }
        else if (strstr(buffer, "@fragment") != nullptr)
        {
            index = (unsigned int)ShaderType::Fragment;
        }
        else
        {
            ss[index] += buffer;
            ss[index] += '\n';
        }
    }
    file.close();
    

    int success;
    char infoLog[512];
    const char* vsource = ss[(size_t)ShaderType::Vertex].c_str();
    const char* fsource = ss[(size_t)ShaderType::Fragment].c_str();

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vsource, NULL);
    glCompileShader(vShader);

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, 0, infoLog);
        std::cout << "Shader Compile Error. " << infoLog << std::endl;
    }

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fsource, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, 0, infoLog);
        std::cout << "Shader Compile Error. " << infoLog << std::endl;
    }

    m_shader = glCreateProgram();
    glAttachShader(m_shader, vShader);
    glAttachShader(m_shader, fShader);
    glLinkProgram(m_shader);

    glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_shader, 512, 0, infoLog);
        std::cout << "Program Link Error. " << infoLog << std::endl;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}