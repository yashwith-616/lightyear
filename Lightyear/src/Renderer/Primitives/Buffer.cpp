#include "Lightyear/Renderer/Primitives/Buffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

#pragma region BufferLayout
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
#pragma endregion

#pragma region VertexBuffer
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(size);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(vertices, size);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
#pragma endregion

#pragma region IndexBuffer
Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLIndexBuffer>(indices, size);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
#pragma endregion

}  // namespace ly::renderer
