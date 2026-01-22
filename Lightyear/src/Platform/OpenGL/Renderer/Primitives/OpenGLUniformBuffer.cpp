#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLUniformBuffer.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace ly::renderer
{

OpenGlUniformBuffer::OpenGlUniformBuffer(std::string name, uint32_t size, uint32_t bindingPoint) :
    UniformBuffer(std::move(name)), m_bindingPoint(bindingPoint), m_size(size)
{
    glCreateBuffers(1, &m_bufferId);
    glNamedBufferData(m_bufferId, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_bufferId);
}

OpenGlUniformBuffer::~OpenGlUniformBuffer() { glDeleteBuffers(1, &m_bufferId); }

void OpenGlUniformBuffer::setData(void const* data, uint32_t size, uint32_t offset)
{
    LY_ASSERT(offset + size <= m_size, "UniformBuffer overflow!");
    glNamedBufferSubData(m_bufferId, offset, size, data);
}

void OpenGlUniformBuffer::bind() const { glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId); }

void OpenGlUniformBuffer::unBind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

void OpenGlUniformBuffer::debug(uint32_t programId, std::string const& blockName)
{
    GLuint const blockIndex = glGetUniformBlockIndex(programId, blockName.c_str());
    if (blockIndex == GL_INVALID_INDEX)
    {
        LY_CORE_LOG(LogType::Error, "UBO block '{}' not found in shader program", blockName);
        return;
    }

    GLint blockSize = 0;
    glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    LY_CORE_LOG(LogType::Info, "UBO '{}' block Size: {} bytes", blockName, blockSize);

    GLint activeUniforms = 0;
    glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniforms);

    std::vector<GLint> uniformIndices(activeUniforms);
    glGetActiveUniformBlockiv(programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformIndices.data());

    std::vector<GLint> offsets(activeUniforms);
    glGetActiveUniformsiv(
        programId,
        activeUniforms,
        reinterpret_cast<GLuint const*>(uniformIndices.data()),
        GL_UNIFORM_OFFSET,
        offsets.data());

    std::vector<GLint> types(activeUniforms);
    glGetActiveUniformsiv(
        programId,
        activeUniforms,
        reinterpret_cast<GLuint const*>(uniformIndices.data()),
        GL_UNIFORM_TYPE,
        types.data());

    std::vector<GLint> sizes(activeUniforms);
    glGetActiveUniformsiv(
        programId,
        activeUniforms,
        reinterpret_cast<GLuint const*>(uniformIndices.data()),
        GL_UNIFORM_SIZE,
        sizes.data());

    constexpr uint32_t bufferSize = 256;
    std::vector<char> nameBuffer(bufferSize);
    std::vector<uint8_t> bufferData(blockSize);

    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
    glGetBufferSubData(GL_UNIFORM_BUFFER, 0, blockSize, bufferData.data());

    LY_CORE_LOG(LogType::Info, "UBO '{}' has {} active uniforms:", blockName, activeUniforms);

    for (GLint i = 0; i < activeUniforms; ++i)
    {
        GLsizei nameLength = 0;
        GLenum uniformType = types[i];

        glGetActiveUniform(
            programId,
            uniformIndices[i],
            static_cast<GLsizei>(nameBuffer.size()),
            &nameLength,
            &sizes[i],
            &uniformType,
            nameBuffer.data());

        char const* name = nameBuffer.data();
        GLint offset = offsets[i];
        GLenum type = types[i];

        LY_CORE_LOG(LogType::Info, "  Name: {}, Offset: {}, Type: 0x{:X}", name, offset, type);

        constexpr int maxPreviewBytes = 4 * 4 * 4;

        auto floatData = reinterpret_cast<float const*>(&bufferData[offset]);
        if (uniformType == GL_FLOAT_MAT4)
        {
            auto mat = reinterpret_cast<float const*>(&bufferData[offset]);

            // std140 stores mat4 column-major by default
            for (int row = 0; row < 4; ++row)
            {
                std::string rowStr;
                for (int col = 0; col < 4; ++col)
                {
                    float value = mat[col * 4 + row]; // column-major indexing
                    std::format_to(std::back_inserter(rowStr), "{: .6f} ", value);
                }
                LY_CORE_LOG(LogType::Info, "Row {}: {}", row, rowStr);
            }
        }
    }
}

} // namespace ly::renderer
