#include "Lightyear/Renderer/Abstract/RendererContext.h"
#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"
#include "Lightyear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

scope<RendererContext> RendererContext::create(void* windowHandle) {
    switch (Renderer::getApi()) {
        case RendererApi::Api::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererApi::Api::OpenGl:
            return makeScope<OpenGlContext>(static_cast<GLFWwindow*>(windowHandle));
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer