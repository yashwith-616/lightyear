#include "Lightyear/Platform/OpenGL/OpenGLTexture2D.h"
#include "Lightyear/Renderer/Primitives/Texture.h"

#include <glad.h>
#include <stb_image.h>

namespace ly::renderer {

OpenGLTexture2D::OpenGLTexture2D(const CPath& path) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    stbi_uc* rawData = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    LY_CORE_ASSERT(rawData, "Failed to load image");

    m_Width  = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureHandle);
    glTextureStorage2D(m_TextureHandle, 1, GetGLInternalFormat(channels), m_Width, m_Height);

    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_TextureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(m_TextureHandle,
                        0,
                        0,
                        0,
                        m_Width,
                        m_Height,
                        GetGLDataFormat(channels),
                        GL_UNSIGNED_BYTE,
                        rawData);

    stbi_image_free(rawData);
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_TextureHandle);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(slot, m_TextureHandle);
}

uint32_t OpenGLTexture2D::GetGLDataFormat(int channels) const {
    switch (channels) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            LY_CORE_ASSERT(false, "Invlaid channels! Image data format not supported by OpenGL");
            return GL_NONE;
    }
}

uint32_t OpenGLTexture2D::GetGLInternalFormat(int channels) const {
    switch (channels) {
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

}  // namespace ly::renderer