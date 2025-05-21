#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Platform/OpenGL/OpenGLFramebuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

ly::Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLFramebuffer>(spec);
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer
