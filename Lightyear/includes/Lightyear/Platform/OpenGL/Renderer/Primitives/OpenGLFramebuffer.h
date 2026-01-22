#pragma once

#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer
{

class LIGHTYEAR_API OpenGlFramebuffer : public Framebuffer
{
public:
    OpenGlFramebuffer(FramebufferSpecification const& spec);
    ~OpenGlFramebuffer() override;

    void invalidate();
    void resize(uint32_t width, uint32_t height) override;

    void bind() override;
    void unbind() override;

    [[nodiscard]] FramebufferSpecification& getSpecification() override { return m_specification; }
    [[nodiscard]] FramebufferSpecification const& getSpecification() const override { return m_specification; };
    [[nodiscard]] uint32_t getColorAttachmentRenderId() const override { return m_colorAttachment; };

private:
    textureHandle m_renderId{0};
    textureHandle m_colorAttachment{0};
    textureHandle m_depthAttachment{0};

    FramebufferSpecification m_specification{};
};

} // namespace ly::renderer
