#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLTexture2D.h"

#include "Lightyear/Renderer/Primitives/Texture.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>

#include "stb_image.h"
LY_DISABLE_WARNINGS_POP

namespace ly::renderer
{

constexpr int k_stbiImageFlipOn = 1;

OpenGlTexture2D::OpenGlTexture2D(std::filesystem::path const& path)
{
    stbi_set_flip_vertically_on_load(k_stbiImageFlipOn);

    int width{};
    int height{};
    int channels{};

    stbi_uc* rawData = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    LY_CORE_ASSERT(rawData, "Failed to load image");

    m_width = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_textureHandle);
    glTextureStorage2D(m_textureHandle, 1, getGlInternalFormat(channels), width, height);

    glTextureParameteri(m_textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(m_textureHandle, 0, 0, 0, width, height, getGlDataFormat(channels), GL_UNSIGNED_BYTE, rawData);

    stbi_image_free(rawData);
}

OpenGlTexture2D::~OpenGlTexture2D() { glDeleteTextures(1, &m_textureHandle); }

void OpenGlTexture2D::bind(uint32_t slot) const { glBindTextureUnit(slot, m_textureHandle); }

uint32_t OpenGlTexture2D::getGlDataFormat(int channels)
{
    switch (channels)
    {
    case 1:
        return GL_RED;
    case 2:
        return GL_RG;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        LY_CORE_ASSERT(false, "Invalid channels! Image data format not supported by OpenGL");
        return GL_NONE;
    }
}

uint32_t OpenGlTexture2D::getGlInternalFormat(int channels)
{
    switch (channels)
    {
    case 1:
        return GL_R8;
    case 2:
        return GL_RG8;
    case 3:
        return GL_RGB8;
    case 4:
        return GL_RGBA8;
    default:
        LY_CORE_ASSERT(false, "Invalid channels! Image storage format not supported by OpenGL");
        return GL_NONE;
    }
}

} // namespace ly::renderer
