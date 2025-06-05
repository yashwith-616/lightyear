#include "Lightyear/Platform/OpenGL/OpenGLVertexArray.h"
#include "Lightyear/Platform/OpenGL/OpenGLBuffer.h"
#include "glad.h"

namespace ly::renderer {

static GLenum ConvertToOpenGLType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
    }

    LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

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

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
    LY_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

    Bind();
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout.GetElements()) {
        GLenum glType = ConvertToOpenGLType(element.Type);

        switch (element.Type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(m_VertexBufferIndex,
                                      element.GetComponentCount(),
                                      glType,
                                      element.IsNormalized ? GL_TRUE : GL_FALSE,
                                      layout.GetStride(),
                                      TO_VOID_PTR(element.Offset));
                m_VertexBufferIndex++;
                break;
            }

            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                uint8_t count = element.GetComponentCount();
                for (uint8_t i = 0; i < count; i++) {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          count,
                                          glType,
                                          element.IsNormalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          TO_VOID_PTR(element.Offset + sizeof(float) * count * i));
                    glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    m_VertexBufferIndex++;
                }
                break;
            }

            default:
                LY_CORE_ASSERT(false, "Unknow ShaderType!");
        }
    }

    m_Vertexbuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}

}  // namespace ly::renderer
