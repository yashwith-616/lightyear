#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create() {
    switch (s_API) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            break;
        case RendererAPI::API::OpenGL:
            return Scope<OpenGLRendererAPI>();
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer