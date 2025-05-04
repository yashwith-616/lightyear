#include "Lightyear/Renderer/Primitives/Buffer.h"
#include "Lightyear/Platform/OpenGL/OpenGLBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {
    CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride() {
    size_t offset = 0;
    m_Stride      = 0;

    for (auto& element : m_Elements) {
        element.Offset = offset;
        offset += element.Size;
        m_Stride += element.Size;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "Renderer::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(size);
    }

    LY_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "Renderer::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(vertices, size);
    }

    LY_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            LY_CORE_ASSERT(false, "Renderer::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLIndexBuffer>(indices, size);
    }

    LY_CORE_ASSERT(false, "Unknown Renderer API!");
    return nullptr;
}

}  // namespace ly::renderer
