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
Ref<VertexBuffer> VertexBuffer::Create(uint32_t sizeInBytes) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(sizeInBytes);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

Ref<VertexBuffer> VertexBuffer::Create(std::span<const float> vertices) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLVertexBuffer>(vertices);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
#pragma endregion

#pragma region IndexBuffer
Ref<IndexBuffer> IndexBuffer::Create(std::span<const uint32_t> indices) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::OpenGL:
            return MakeRef<OpenGLIndexBuffer>(indices);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
#pragma endregion

}  // namespace ly::renderer
