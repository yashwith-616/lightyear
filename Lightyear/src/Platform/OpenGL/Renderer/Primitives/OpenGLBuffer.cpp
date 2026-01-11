#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

/////////////////////////////////////////////////////////
// OpenGL Vertex Buffer
/////////////////////////////////////////////////////////
namespace ly::renderer
{

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(std::span<const float> vertices)
{
    const auto verticesSize = narrow_cast<GLsizeiptr>(vertices.size_bytes());

    glCreateBuffers(1, &m_RenderID);
    LY_CORE_ASSERT(m_RenderID > 0, "VertexBuffer render ID is not initialized");
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RenderID); }

void OpenGLVertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RenderID); }

void OpenGLVertexBuffer::unBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

} // namespace ly::renderer

/////////////////////////////////////////////////////////
// OpenGL Index Buffer
/////////////////////////////////////////////////////////
namespace ly::renderer
{

OpenGLIndexBuffer::OpenGLIndexBuffer(std::span<const uint32_t> indices) : m_Count(indices.size())
{
    glCreateBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);

    const auto bufferSize = narrow_cast<GLsizeiptr>(indices.size_bytes());
    glBufferData(GL_ARRAY_BUFFER, bufferSize, indices.data(), GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RenderID); }

void OpenGLIndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID); }

void OpenGLIndexBuffer::unBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace ly::renderer
