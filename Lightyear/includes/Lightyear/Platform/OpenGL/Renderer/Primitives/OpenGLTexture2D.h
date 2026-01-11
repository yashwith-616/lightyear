#pragma once

#include "Lightyear/Renderer/Primitives/Texture.h"

namespace ly::renderer
{

class LIGHTYEAR_API OpenGLTexture2D : public Texture2D
{
public:
    explicit OpenGLTexture2D(const std::filesystem::path& path);
    ~OpenGLTexture2D() override;

    void bind(uint32_t slot = 0) const override;
    [[nodiscard]] uint32_t getWidth() const override { return m_Width; }
    [[nodiscard]] uint32_t getHeight() const override { return m_Height; }

private:
    // TODO: Remove when asset manager is written
    std::filesystem::path m_Path{};
    uint32_t m_Width{};
    uint32_t m_Height{};

    textureHandle m_TextureHandle{0};

    [[nodiscard]] static uint32_t GetGLDataFormat(int channels);
    [[nodiscard]] static uint32_t GetGLInternalFormat(int channels);
};

} // namespace ly::renderer
