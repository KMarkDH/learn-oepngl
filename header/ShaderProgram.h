// #include <iostream>
// #include <glm/glm.hpp>

// class ShaderProgram
// {
// public:

//     ShaderProgram();
//     explicit ShaderProgram(std::string);
//     ~ShaderProgram();

//     void use();
//     void u4f(std::string uniform, float x, float y, float z, float w);
//     void uf(std::string uniform, float x);
//     void ui(std::string uniform, int x);
//     void umat4(std::string uniform, glm::mat4& mat);

// protected:

//     enum class ShaderType
//     {
//         Vertex = 0,
//         Fragment = 1
//     };

//     unsigned int m_pId;
//     int readShaderFile(std::string);
//     int genShader(unsigned int type, const char *content);
//     int genProgram(unsigned int vs, unsigned int fs);
// };