#include "Lightyear/Renderer/Primitives/Buffer.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly::renderer {

#pragma region BufferLayout
BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : m_elements(elements) {
    calculateOffsetsAndStride();
}

void BufferLayout::calculateOffsetsAndStride() {
    size_t offset = 0;
    m_stride      = 0;

    for (auto& element : m_elements) {
        element.offset = offset;
        offset += element.size;
        m_stride += element.size;
    }
}
#pragma endregion

#pragma region VertexBuffer
ref<VertexBuffer> VertexBuffer::create(uint32_t sizeInBytes) {
    switch (Renderer::getApi()) {
        case RendererApi::Api::OpenGl:
            return makeRef<OpenGlVertexBuffer>(sizeInBytes);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}

ref<VertexBuffer> VertexBuffer::create(std::span<float const> vertices) {
    switch (Renderer::getApi()) {
        case RendererApi::Api::OpenGl:
            return makeRef<OpenGlVertexBuffer>(vertices);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
void VertexBuffer::bind() const {}
void VertexBuffer::unBind() const {}
#pragma endregion

#pragma region IndexBuffer
ref<IndexBuffer> IndexBuffer::create(std::span<uint32_t const> indices) {
    switch (Renderer::getApi()) {
        case RendererApi::Api::OpenGl:
            return makeRef<OpenGlIndexBuffer>(indices);
        default:
            LY_CORE_ASSERT(false, "Invalid API Type is currently not supported!");
            return nullptr;
    }
}
#pragma endregion

}  // namespace ly::renderer
