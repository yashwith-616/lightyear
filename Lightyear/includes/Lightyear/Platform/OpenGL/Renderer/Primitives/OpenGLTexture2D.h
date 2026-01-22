#pragma once

#include "Lightyear/Renderer/Primitives/Texture.h"

namespace ly::renderer
{

class LIGHTYEAR_API OpenGlTexture2D : public Texture2D
{
public:
    explicit OpenGlTexture2D(std::filesystem::path const& path);
    ~OpenGlTexture2D() override;

    void bind(uint32_t slot = 0) const override;
    [[nodiscard]] uint32_t getWidth() const override { return m_width; }
    [[nodiscard]] uint32_t getHeight() const override { return m_height; }

private:
    // TODO: Remove when asset manager is written
    std::filesystem::path m_path{};
    uint32_t m_width{};
    uint32_t m_height{};

    textureHandle m_textureHandle{0};

    [[nodiscard]] static uint32_t getGlDataFormat(int channels);
    [[nodiscard]] static uint32_t getGlInternalFormat(int channels);
};

} // namespace ly::renderer
