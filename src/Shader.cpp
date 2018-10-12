#include "Shader.h"
#include "OGL.h"

Shader::Shader()
:m_shader(-1)
{

}

Shader::~Shader()
{

}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
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
    
    m_shader = program;
    return program;
}

unsigned int Shader::getShader()
{
    return m_shader;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source)
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