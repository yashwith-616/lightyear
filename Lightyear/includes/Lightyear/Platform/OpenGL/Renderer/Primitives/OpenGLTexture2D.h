#pragma once

#include "Lightyear/Renderer/Primitives/Texture.h"

namespace ly::renderer {

class LIGHTYEAR_API OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const CPath& path);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot = 0) const override;
    [[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
    [[nodiscard]] uint32_t GetHeight() const override { return m_Height; }

private:
    // TODO: Remove when asset manager is written
    CPath m_Path{};
    uint32_t m_Width{};
    uint32_t m_Height{};

    TextureHandle m_TextureHandle{ 0 };

    [[nodiscard]] uint32_t GetGLDataFormat(int channels) const;
    [[nodiscard]] uint32_t GetGLInternalFormat(int channels) const;
};

}  // namespace ly::renderer