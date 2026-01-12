#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::renderer {

struct FramebufferSpecification {
    uint32_t width{ k_kDefaultWindowSize.x };
    uint32_t height{ k_kDefaultWindowSize.y };
    uint32_t samples{ 1 };
    bool swapChainTarget{ false };
};

class LIGHTYEAR_API Framebuffer {
public:
    static ref<Framebuffer> create(FramebufferSpecification const& spec);

    virtual ~Framebuffer() = default;
    virtual void resize(uint32_t width, uint32_t height);

    virtual void bind()   = 0;
    virtual void unbind() = 0;

    [[nodiscard]] virtual FramebufferSpecification& getSpecification()             = 0;
    [[nodiscard]] virtual FramebufferSpecification const& getSpecification() const = 0;
    [[nodiscard]] virtual uint32_t getColorAttachmentRenderId() const              = 0;
};

}  // namespace ly::renderer