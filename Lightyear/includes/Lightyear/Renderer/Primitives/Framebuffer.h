#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

struct FramebufferSpecification {
    uint32_t Width{ kDefaultWindowSize.x };
    uint32_t Height{ kDefaultWindowSize.y };
    uint32_t Samples{ 1 };
    bool SwapChainTarget{ false };
};

class LIGHTYEAR_API Framebuffer {
public:
    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

    virtual ~Framebuffer()                               = default;
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void Bind()   = 0;
    virtual void Unbind() = 0;

    [[nodiscard]] virtual FramebufferSpecification& GetSpecification()             = 0;
    [[nodiscard]] virtual const FramebufferSpecification& GetSpecification() const = 0;
    [[nodiscard]] virtual uint32_t GetColorAttachmentRenderID() const              = 0;
};

}  // namespace ly::renderer