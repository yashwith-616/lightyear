#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLFramebuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLFramebuffer>(spec);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

}  // namespace ly::renderer
