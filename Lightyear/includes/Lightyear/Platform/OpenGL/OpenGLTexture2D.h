#pragma once

#include <lypch.h>
#include "Lightyear/Renderer/Primitives/Texture.h"

namespace ly::renderer {

class LIGHTYEAR_API OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(const CPath& path);
    virtual ~OpenGLTexture2D();

    FORCEINLINE virtual void Bind(uint32_t slot = 0) const override;

    FORCEINLINE virtual uint32_t GetWidth() const override { return m_Width; }
    FORCEINLINE virtual uint32_t GetHeight() const override { return m_Height; }

private:
    // TODO: Remove when asset manager is written
    CPath m_Path{};
    uint32_t m_Width{};
    uint32_t m_Height{};

    TextureHandle m_TextureHandle{ 0 };

private:
    FORCEINLINE uint32_t GetGLDataFormat(int channels) const;
    FORCEINLINE uint32_t GetGLInternalFormat(int channels) const;
};

}  // namespace ly::renderer