#ifndef TEXTURE_H
#define TEXTURE_H

#include <Application.h>
#define SYNTHESIZE(type, var) \
private: \
type var; \
public: \
void set##var(type _var) { var = _var; }\
type get##var() { return var; }

enum class TextureType
{
    CLAMP_BOARD_LINEAR = 0,
    CLAMP_EDGT_LINEAR,
    REPEAT_LINEAR,
    MIRRORED_REPEAT_LINEAR,
    CLAMP_BOARD_NEAREST,
    CLAMP_EDGT_NEAREST,
    REPEAT_NEAREST,
    MIRRORED_REPEAT_NEAREST,
    CLAMP_BOARD_MIPMAP,
    CLAMP_EDGT_MIPMAP,
    REPEAT_MIPMAP,
    MIRRORED_REPEAT_MIPMAP,
};

class tTexture
{
    SYNTHESIZE(unsigned int, pointer);

public:

    tTexture() = delete;
    explicit tTexture(std::string filename, unsigned int pointer, TextureType type);

    void bind() const;

    ~tTexture();

private:

    unsigned int m_texture;
    TextureType m_type;
};

#endif