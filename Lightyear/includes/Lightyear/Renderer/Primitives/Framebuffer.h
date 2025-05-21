#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {

struct FramebufferSpecification {
    uint32_t Width{ DEFAULT_WINDOW_WIDTH };
    uint32_t Height{ DEFAULT_WINDOW_HEIGHT };
    uint32_t Samples{ 1 };
    bool SwapChainTarget{ false };
};

class LIGHTYEAR_API Framebuffer {
public:
    static ly::Ref<Framebuffer> Create(const FramebufferSpecification& spec);

    virtual ~Framebuffer()                               = default;
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual FramebufferSpecification& GetSpecification()             = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;
    virtual uint32_t GetColorAttachmentRenderID() const              = 0;

    virtual void Bind()   = 0;
    virtual void Unbind() = 0;
};

}  // namespace ly::renderer