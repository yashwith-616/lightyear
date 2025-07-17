#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLUniformBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad.h>
LY_DISABLE_WARNINGS_POP

namespace ly::renderer {

OpenGLUniformBuffer::OpenGLUniformBuffer(std::string name, uint32_t size, uint32_t bindingPoint)
    : UniformBuffer(std::move(name)), m_BindingPoint(bindingPoint) {
    glCreateBuffers(1, &m_BufferID);
    glNamedBufferData(m_BufferID, size, nullptr, GL_DYNAMIC_DRAW);  // TODO: investigate usage hint
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_BufferID);
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

void OpenGLUniformBuffer::Debug(uint32_t programID, const std::string& blockName) {
    const GLuint blockIndex = glGetUniformBlockIndex(programID, blockName.c_str());
    if (blockIndex == GL_INVALID_INDEX) {
        LY_CORE_LOG(LogType::Error, "UBO block '{}' not found in shader program", blockName);
        return;
    }

    GLint blockSize = 0;
    glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    LY_CORE_LOG(LogType::Info, "UBO '{}' block Size: {} bytes", blockName, blockSize);

    GLint activeUniforms = 0;
    glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniforms);

    std::vector<GLint> uniformIndices(activeUniforms);
    glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices.data());

    std::vector<GLint> offsets(activeUniforms);
    glGetActiveUniformsiv(programID,
                          activeUniforms,
                          reinterpret_cast<const GLuint*>(uniformIndices.data()),
                          GL_UNIFORM_OFFSET,
                          offsets.data());

    std::vector<GLint> types(activeUniforms);
    glGetActiveUniformsiv(programID,
                          activeUniforms,
                          reinterpret_cast<const GLuint*>(uniformIndices.data()),
                          GL_UNIFORM_TYPE,
                          types.data());

    std::vector<GLint> sizes(activeUniforms);
    glGetActiveUniformsiv(programID,
                          activeUniforms,
                          reinterpret_cast<const GLuint*>(uniformIndices.data()),
                          GL_UNIFORM_SIZE,
                          sizes.data());

    constexpr uint32_t bufferSize = 256;
    std::vector<char> nameBuffer(bufferSize);
    std::vector<uint8_t> bufferData(blockSize);

    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glGetBufferSubData(GL_UNIFORM_BUFFER, 0, blockSize, bufferData.data());

    LY_CORE_LOG(LogType::Info, "UBO '{}' has {} active uniforms:", blockName, activeUniforms);

    for (GLint i = 0; i < activeUniforms; ++i) {
        GLsizei nameLength = 0;
        GLenum uniformType = types[i];

        glGetActiveUniform(programID,
                           uniformIndices[i],
                           static_cast<GLsizei>(nameBuffer.size()),
                           &nameLength,
                           &sizes[i],
                           &uniformType,
                           nameBuffer.data());

        const char* name = nameBuffer.data();
        GLint offset     = offsets[i];
        GLenum type      = types[i];

        LY_CORE_LOG(LogType::Info, "  Name: {}, Offset: {}, Type: 0x{:X}", name, offset, type);

        constexpr int maxPreviewBytes = 16;

        // Dump value bytes (up to kMaxPreviewBytes)
        std::string hexDump;
        const int availableBytes = blockSize - offset;
        const int bytesToDump    = std::min(maxPreviewBytes, availableBytes);

        for (int j = 0; j < bytesToDump; ++j) {
            std::format_to(std::back_inserter(hexDump), "{:02X} ", static_cast<uint8_t>(bufferData[offset + j]));
        }

        LY_CORE_LOG(LogType::Info, "    Raw bytes: {}", hexDump);
    }
}

void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
    glNamedBufferSubData(m_BufferID, offset, size, data);
}

}  // namespace ly::renderer
