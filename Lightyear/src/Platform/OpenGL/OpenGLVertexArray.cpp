#include "Lightyear/Platform/OpenGL/OpenGLVertexArray.h"
#include "Lightyear/Platform/OpenGL/OpenGLBuffer.h"
#include "glad.h"

namespace ly::renderer {

OpenGLVertexArray::OpenGLVertexArray() {
    glCreateVertexArrays(1, &m_RenderID);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &m_RenderID);
}

void OpenGLVertexArray::Bind() const {
    glBindVertexArray(m_RenderID);
}

void OpenGLVertexArray::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer) {
    // TODO: Implementation pending
}

void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer& indexBuffer) {
    glBindVertexArray(m_RenderID);
    indexBuffer.Bind();

    *m_IndexBuffer = indexBuffer;
}

}  // namespace ly::renderer
