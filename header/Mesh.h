#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "Shader.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{

public:

    std::vector<Vertex> m_vertices;
    std::vector<Texture> m_textures;
    std::vector<unsigned int> m_indices;

    explicit Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
    void draw(Shader &shader);

private:

    unsigned int m_vao, m_vbo, m_ibo;
    void setupMesh();
};