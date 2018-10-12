#ifndef SHADER_H
#define SHADER_H
#include <iostream>

class Shader
{
public:

    Shader();
    ~Shader();
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int getShader();

protected:

    unsigned int compileShader(unsigned int type, const std::string& source);

private:

    unsigned int m_shader;
};

#endif