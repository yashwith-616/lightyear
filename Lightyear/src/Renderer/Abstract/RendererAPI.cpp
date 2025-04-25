#include "Lightyear/Renderer/Abstract/RendererAPI.h"
#include "Lightyear/Renderer/Primitives/Buffer.h"

namespace ly::renderer {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

RendererAPI RendererAPI::Create() {
    return RendererAPI();

    switch (s_API) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        case RendererAPI::API::OpenGL: new OpenGLRendererAPI();
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer