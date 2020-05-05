#include "Model.h"
#include <stb_image.h>

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
#if defined(_WIN32) || defined(WIN32)
    m_directory = path.substr(0, path.find_last_of('\\'));
#endif
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.Position = position;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 tCoord;
            tCoord.x = mesh->mTextureCoords[0][i].x;
            tCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = tCoord;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end())
        ;
    }

    return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::loadMaterialTexture(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), m_directory);
        texture.type = typeName;

        texture.path = str.C_Str();
        textures.push_back(texture);
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char* path, std::string directory)
{
    std::string filePath = std::string(path);
#if defined(_WIN32) || defined(WIN32)
    filePath = directory + '\\' + filePath;
#else
    filePath = directory + '/' + filePath;
#endif

    std::cout << "load texture " << filePath << std::endl;

    unsigned int id;
    glGenTextures(1, &id);

    int width, height, channel;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channel, 0);
    if (data)
    {
        GLenum format;
        if (channel == 1)
            format = GL_RED;
        else if (channel == 3)
            format = GL_RGB;
        else if (channel == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::clog << "Texture failed to load at path: " << m_directory << path << std::endl;
        stbi_image_free(data);
    }
    
    return id;
}

Model::Model(const char* path)
{
    loadModel(path);
}

void Model::Draw(Shader shader)
{
    for (unsigned int i = 0; i < m_meshes.size(); ++i)
        m_meshes[i].draw(shader);
}

Model::~Model()
{

}
