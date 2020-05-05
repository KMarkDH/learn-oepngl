#include <iostream>
#include <vector>
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene* scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, std::string directory);

public:
    explicit Model(const char* path);
    void Draw(Shader shader);
    ~Model();
};