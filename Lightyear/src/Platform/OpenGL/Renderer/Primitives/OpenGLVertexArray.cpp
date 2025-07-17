#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLVertexArray.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad.h>
LY_DISABLE_WARNINGS_POP

namespace {
using ly::renderer::ShaderDataType;
GLenum GetOpenGLType(const ShaderDataType type) {
    switch (type) {
        case ShaderDataType::FLOAT:
        case ShaderDataType::FLOAT2:
        case ShaderDataType::FLOAT3:
        case ShaderDataType::FLOAT4:
        case ShaderDataType::MAT3:
        case ShaderDataType::MAT4:
            return GL_FLOAT;
        case ShaderDataType::INT:
        case ShaderDataType::INT2:
        case ShaderDataType::INT3:
        case ShaderDataType::INT4:
            return GL_INT;
        case ShaderDataType::BOOL:
            return GL_BOOL;
        case ShaderDataType::NONE:
            LY_CORE_ASSERT(false, "ShaderDataType::None is not a valid type!");
            return GL_INVALID_ENUM;
        default:
            LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return GL_INVALID_ENUM;
    }
}
}  // namespace

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

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
    LY_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout");

    Bind();
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout.GetElements()) {
        const GLenum glType = GetOpenGLType(element.Type);

        switch (element.Type) {
            case ShaderDataType::FLOAT:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::FLOAT4:
            case ShaderDataType::INT:
            case ShaderDataType::INT2:
            case ShaderDataType::INT3:
            case ShaderDataType::INT4:
            case ShaderDataType::BOOL: {
                const auto strideSize        = narrow_cast<GLsizei>(layout.GetStride());
                const auto componentCount    = narrow_cast<GLint>(element.GetComponentCount());
                const GLboolean isNormalized = element.IsNormalized ? GL_TRUE : GL_FALSE;

                glEnableVertexAttribArray(m_VertexBufferIndex);
                glVertexAttribPointer(
                    m_VertexBufferIndex, componentCount, glType, isNormalized, strideSize, AsVoidPtr(element.Offset));
                m_VertexBufferIndex++;
                break;
            }

            case ShaderDataType::MAT3:
            case ShaderDataType::MAT4: {
                const auto strideSize        = narrow_cast<GLsizei>(layout.GetStride());
                const auto componentCount    = narrow_cast<GLint>(element.GetComponentCount());
                const GLboolean isNormalized = element.IsNormalized ? GL_TRUE : GL_FALSE;

                for (int i = 0; i < componentCount; i++) {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                          componentCount,
                                          glType,
                                          isNormalized,
                                          strideSize,
                                          AsVoidPtr(element.Offset + (sizeof(float) * componentCount * i)));
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
