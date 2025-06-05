#pragma once

#include "Lightyear/Renderer/Primitives/Framebuffer.h"

namespace ly::renderer {

class LIGHTYEAR_API OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    void Invalidate();
    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual FramebufferSpecification& GetSpecification() { return m_Specification; }
    virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; };
    virtual uint32_t GetColorAttachmentRenderID() const { return m_ColorAttachment; };

    virtual void Bind() override;
    virtual void Unbind() override;

private:
    TextureHandle m_RenderID{ 0 };
    TextureHandle m_ColorAttachment{ 0 };
    TextureHandle m_DepthAttachment{ 0 };

    FramebufferSpecification m_Specification{};
};

}  // namespace ly::renderer