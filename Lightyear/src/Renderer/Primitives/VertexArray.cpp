#include "Lightyear/Renderer/Primitives/VertexArray.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLVertexArray.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"
#include "Lightyear/Renderer/Abstract/RendererAPI.h"

namespace ly::renderer
{

ref<VertexArray> VertexArray::create()
{
    switch (Renderer::getApi())
    {
    case RendererApi::Api::OpenGl:
        return makeRef<OpenGlVertexArray>();
    default:
        LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
        return nullptr;
    }
}
void VertexArray::unbind() const {}
void VertexArray::addVertexBuffer(ref<VertexBuffer> const& vertexBuffer) {}
void VertexArray::setIndexBuffer(ref<IndexBuffer> const& indexBuffer) {}

} // namespace ly::renderer
