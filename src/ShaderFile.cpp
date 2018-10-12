#include "ShaderFile.h"
#include <fstream>
#include <sstream>

ShaderFile::ShaderFile(const char* file_path)
{
    std::ifstream file(file_path, std::ios::in);
    if (!file.is_open())
    {
        std::cout << "Failed to open file : " << file_path << std::endl;
    }

    std::stringstream ss[2];
    Type type;
    while(!file.eof())
    {
        std::string line;
        getline(file, line); 
        
        if (line == "#vertex shader")
        {
            type = Type::Vertex_Shader;
            continue;
        }
        else if (line == "#fragment shader")
        {
            type = Type::Fragment_Shader;
            continue;
        }

        if (Type::Default != type)
            ss[(int)type] << line << "\n";
    }

    m_shaderArrary[0] = ss[0].str();
    m_shaderArrary[1] = ss[1].str();
}

ShaderFile::~ShaderFile()
{

}

const char* ShaderFile::getShader(ShaderFile::Type type)
{
    if (m_shaderArrary[(int)type].length() > 0)
    {
        return m_shaderArrary[(int)type].c_str();
    }
    return "";
}