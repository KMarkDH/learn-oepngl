#ifndef SHADER_H
#define SHADER_H
#include <iostream>

class Shader
{
public:

    Shader();
    ~Shader();
    int createShader(const std::string& vertexShader, const std::string& fragmentShader);
    int getShader();

protected:

    unsigned int compileShader(unsigned int type, const std::string& source);

private:

    int m_shader;
};

#endif