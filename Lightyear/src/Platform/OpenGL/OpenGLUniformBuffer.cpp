#include "Lightyear/Platform/OpenGL/OpenGLUniformBuffer.h"
#include "glad.h"

namespace ly::renderer {

OpenGLUniformBuffer::OpenGLUniformBuffer(const CName& name, uint32_t size, uint32_t bindingPoint)
    : m_BindingPoint(bindingPoint) {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, m_BufferID, 0, size);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() {
    glDeleteBuffers(1, &m_BufferID);
}

void OpenGLUniformBuffer::Bind() const {
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
}

void OpenGLUniformBuffer::UnBind() const {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}  // namespace ly::renderer
