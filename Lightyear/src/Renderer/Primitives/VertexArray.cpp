#include "Lightyear/Renderer/Primitives/VertexArray.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLVertexArray.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"

namespace ly::renderer
{

Ref<VertexArray> VertexArray::create()
{
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OpenGL:
        return MakeRef<OpenGLVertexArray>();
    default:
        LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
        return nullptr;
    }
}
void VertexArray::bind() const {}

} // namespace ly::renderer
