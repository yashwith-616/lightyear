#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLVertexArray.h"
#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace {
using ly::renderer::ShaderDataType;
GLenum getOpenGlType(ShaderDataType const type) {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::None:
            LY_CORE_ASSERT(false, "ShaderDataType::None is not a valid type!");
            return GL_INVALID_ENUM;
        default:
            LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return GL_INVALID_ENUM;
    }
}
}  // namespace

namespace ly::renderer {

OpenGlVertexArray::OpenGlVertexArray() {
    glCreateVertexArrays(1, &m_renderId);
}

OpenGlVertexArray::~OpenGlVertexArray() {
    glDeleteVertexArrays(1, &m_renderId);
}

void OpenGlVertexArray::bind() const {
    glBindVertexArray(m_renderId);
}

void OpenGlVertexArray::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGlVertexArray::addVertexBuffer(ref<VertexBuffer> const& vertexBuffer) {
    LY_CORE_ASSERT(!vertexBuffer->getLayout().getElements().empty(), "Vertex Buffer has no layout");

    bind();
    vertexBuffer->bind();

    auto const& layout = vertexBuffer->getLayout();
    for (auto const& element : layout.getElements()) {
        GLenum const glType = getOpenGlType(element.type);

        switch (element.type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                auto const strideSize        = narrowCast<GLsizei>(layout.getStride());
                auto const componentCount    = narrowCast<GLint>(element.getComponentCount());
                GLboolean const isNormalized = element.isNormalized ? GL_TRUE : GL_FALSE;

                glEnableVertexAttribArray(m_vertexBufferIndex);
                glVertexAttribPointer(
                    m_vertexBufferIndex, componentCount, glType, isNormalized, strideSize, asVoidPtr(element.offset));
                m_vertexBufferIndex++;
                break;
            }

            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                auto const strideSize        = narrowCast<GLsizei>(layout.getStride());
                auto const componentCount    = narrowCast<GLint>(element.getComponentCount());
                GLboolean const isNormalized = element.isNormalized ? GL_TRUE : GL_FALSE;

                for (int i = 0; i < componentCount; i++) {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribPointer(m_vertexBufferIndex,
                                          componentCount,
                                          glType,
                                          isNormalized,
                                          strideSize,
                                          asVoidPtr(element.offset + (sizeof(float) * componentCount * i)));
                    glVertexAttribDivisor(m_vertexBufferIndex, 1);
                    m_vertexBufferIndex++;
                }
                break;
            }
            default:
                LY_CORE_ASSERT(false, "Unknow ShaderType!");
        }
    }

    m_vertexbuffers.push_back(vertexBuffer);
}

void OpenGlVertexArray::setIndexBuffer(ref<IndexBuffer> const& indexBuffer) {
    glBindVertexArray(m_renderId);
    indexBuffer->bind();
    m_indexBuffer = indexBuffer;
}

}  // namespace ly::renderer
