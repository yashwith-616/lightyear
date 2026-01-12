#include "Lightyear/Renderer/Primitives/Framebuffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLFramebuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

ref<Framebuffer> Framebuffer::create(FramebufferSpecification const& spec) {
    switch (Renderer::getApi()) {
        case RendererApi::Api::OpenGl:
            return makeRef<OpenGlFramebuffer>(spec);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
void Framebuffer::resize(uint32_t width, uint32_t height) {}

}  // namespace ly::renderer
