#include "Lightyear/Renderer/Primitives/VertexArray.h"
#include "Lightyear/Platform/OpenGL/OpenGLVertexArray.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"

namespace ly::renderer {

VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
    }

    LY_CORE_ASSERT(false, "Invalid RendererAPI!");
    return nullptr;
}

}  // namespace ly::renderer