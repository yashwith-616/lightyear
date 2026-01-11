#pragma once

#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer
{

class LIGHTYEAR_API OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer() override;

    void Invalidate();
    void Resize(uint32_t width, uint32_t height) override;

    void Bind() override;
    void Unbind() override;

    [[nodiscard]] FramebufferSpecification& GetSpecification() override { return m_Specification; }
    [[nodiscard]] const FramebufferSpecification& GetSpecification() const override { return m_Specification; };
    [[nodiscard]] uint32_t GetColorAttachmentRenderID() const override { return m_ColorAttachment; };

private:
    textureHandle m_RenderID{0};
    textureHandle m_ColorAttachment{0};
    textureHandle m_DepthAttachment{0};

    FramebufferSpecification m_Specification{};
};

} // namespace ly::renderer
