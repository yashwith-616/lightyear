#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

/////////////////////////////////////////////////////////
// OpenGL Vertex Buffer
/////////////////////////////////////////////////////////
namespace ly::renderer {

OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size) {
    glCreateBuffers(1, &m_renderId);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGlVertexBuffer::OpenGlVertexBuffer(std::span<float const> vertices) {
    auto const verticesSize = narrowCast<GLsizeiptr>(vertices.size_bytes());

    glCreateBuffers(1, &m_renderId);
    LY_CORE_ASSERT(m_renderId > 0, "VertexBuffer render ID is not initialized");
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);
}

OpenGlVertexBuffer::~OpenGlVertexBuffer() {
    glDeleteBuffers(1, &m_renderId);
}

void OpenGlVertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
}

void OpenGlVertexBuffer::unBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGlVertexBuffer::setData(void const* data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

}  // namespace ly::renderer

/////////////////////////////////////////////////////////
// OpenGL Index Buffer
/////////////////////////////////////////////////////////
namespace ly::renderer {

OpenGlIndexBuffer::OpenGlIndexBuffer(std::span<uint32_t const> indices) : m_count(indices.size()) {
    glCreateBuffers(1, &m_renderId);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderId);

    auto const bufferSize = narrowCast<GLsizeiptr>(indices.size_bytes());
    glBufferData(GL_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
}

OpenGlIndexBuffer::~OpenGlIndexBuffer() {
    glDeleteBuffers(1, &m_renderId);
}

void OpenGlIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId);
}

void OpenGlIndexBuffer::unBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace ly::renderer