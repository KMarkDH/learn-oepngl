#include <glad/glad.h>
#include <ShaderProgram.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

extern std::string proc_path;
extern std::string proc_name;
extern void LOG(std::initializer_list<std::string>);

ShaderProgram::ShaderProgram() {
    
}

ShaderProgram::ShaderProgram(std::string shaderFile)
:m_pId(-1)
{
    char dlm = '/';
#if defined(_WIN32) || defined(WIN32)
    dlm = '\\';
#endif
    m_pId = this->readShaderFile(proc_path + dlm + "res/shaders/" + shaderFile + ".shader");
}

ShaderProgram::~ShaderProgram()
{
    if (m_pId < 0) return;
    glDeleteProgram(m_pId);
}

void ShaderProgram::use()
{
    if (m_pId < 0) {return;}
    glUseProgram(m_pId);
}

void ShaderProgram::u4f(std::string uniform, float x, float y, float z, float w)
{
    unsigned int unf = glGetUniformLocation(m_pId, uniform.c_str());
    glUniform4f(unf, x, y, z, w);
}

void ShaderProgram::uf(std::string uniform, float x)
{
    unsigned int unf = glGetUniformLocation(m_pId, uniform.c_str());
    glUniform1f(unf, x);
}

void ShaderProgram::ui(std::string uniform, int x)
{
    unsigned int unf = glGetUniformLocation(m_pId, uniform.c_str());
    glUniform1i(unf, x);
}

void ShaderProgram::umat4(std::string uniform, glm::mat4& mat)
{
    unsigned int unf = glGetUniformLocation(m_pId, uniform.c_str());
    glUniformMatrix4fv(unf, 1, GL_FALSE, glm::value_ptr(mat));
}

int ShaderProgram::readShaderFile(std::string filePath)
{
    std::fstream f(filePath, std::ios::in);
    if (!f.is_open()) 
    {   
        LOG({"Failed to load shader file : ", filePath});
        return -1;
    }

    std::string shader[2];
    unsigned int index;

    char buffer[512];

    while (!f.eof())
    {
        f.getline(buffer, 512);
        if (strstr(buffer, "@vertex") != nullptr)
            index = (unsigned int)ShaderType::Vertex;
        else if(strstr(buffer, "@fragment") != nullptr)
            index = (unsigned int)ShaderType::Fragment;
        else
        {
            shader[index] += buffer;
            shader[index] += '\n';
        }
    }
    f.close();

    int vs = genShader(GL_VERTEX_SHADER, shader[(unsigned int)ShaderType::Vertex].c_str());
    int fs = genShader(GL_FRAGMENT_SHADER, shader[(unsigned int)ShaderType::Fragment].c_str());
    
    return genProgram(vs, fs);
}

int ShaderProgram::genShader(unsigned int type, const char* content)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &content, NULL);

    int success;
    char log[128];

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 128, NULL, log);
        LOG({"compile shader error" , log});
        return -1;
    }
    return shader;
}

int ShaderProgram::genProgram(unsigned int vs, unsigned int fs)
{
    unsigned program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    char log[128];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 128, NULL, log);
        LOG({"link shader error" , log});
        return -1;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}