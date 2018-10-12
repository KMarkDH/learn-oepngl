#ifndef SHADERFILE_H
#define SHADERFILE_H
#include <iostream>

class ShaderFile
{
public:

    enum Type {
        Default = -1,
        Vertex_Shader = 0,
        Fragment_Shader = 1
    };

    ShaderFile(const char* file_path);
    ~ShaderFile();

    const char* getShader(Type type);

private:

    std::string m_shaderArrary[2];
};

#endif