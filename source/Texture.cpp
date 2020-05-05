#include <Texture.h>
#include <stb_image.h>

#define TEXTURE_LOCATION(pot) (GL_TEXTURE0 + pot)

//偷懒直接把初始化写在构造函数里
tTexture::tTexture(std::string filename, unsigned int pointer, TextureType type)
:m_type(type)
,pointer(pointer)
{
    glGenTextures(1, &m_texture);

    glActiveTexture(TEXTURE_LOCATION(pointer));
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    std::string filepath = Application::getFilePath("/res/textures/" + filename);
    unsigned char* data;
    int width, height, channel;
    data = stbi_load(filepath.c_str(), &width, &height, &channel, 0);
    if (data)
    {
        if (filename.find(".png") != std::string::npos)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (filename.find(".jpg") != std::string::npos)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        std::clog << "Failed to load image : " << filepath << std::endl;
    }
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void tTexture::bind() const
{
    glActiveTexture(TEXTURE_LOCATION(pointer));
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

tTexture::~tTexture()
{

}
