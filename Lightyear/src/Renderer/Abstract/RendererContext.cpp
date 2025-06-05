#include "Lightyear/Renderer/Abstract/RendererContext.h"
#include "Lightyear/Platform/OpenGL/OpenGLContext.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

ly::Scope<RendererContext> RendererContext::Create(void* windowHandle) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return ly::MakeScope<OpenGLContext>(static_cast<GLFWwindow*>(windowHandle));
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer