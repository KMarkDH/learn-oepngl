#ifndef SHADER_H
#define SHADER_H
#include <Application.h>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader() = delete;
    Shader(std::string name);

    ~Shader();

    void use();
    void setInt(std::string uniform, int value);
    void setFloat(std::string uniform, float value);
    void setMat4(std::string uniform, glm::mat4 value);
    void setVec3(std::string uniform, glm::vec3 value);

private:

    unsigned int m_shader;
    void createShader(std::string fileName);
};

#endif //SHADER_H
