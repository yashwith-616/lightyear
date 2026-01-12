#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLShader.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace {
using ly::renderer::ShaderType;

GLenum getGlShaderType(ShaderType shaderType) {
    switch (shaderType) {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Pixel:
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Compute:
            return GL_COMPUTE_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::TessControl:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TessEvaluation:
            return GL_TESS_EVALUATION_SHADER;
        default:
            LY_CORE_ASSERT(false, "Invalid Shader Type!");
            return GL_INVALID_ENUM;
    }
}
}  // namespace

namespace ly::renderer {

OpenGlShader::OpenGlShader(std::string name, std::unordered_map<ShaderType, cPath> const& shaderFiles)
    : m_shaderHandle(glCreateProgram()), m_name(std::move(name)) {
    std::array<shaderHandle, k_shaderTypeCount> shaderHandles = {};
    for (auto const& [shaderType, path] : shaderFiles) {
        std::string shaderSrc = readFile(path);
        auto const index      = static_cast<size_t>(shaderType);
        shaderHandles[index]  = compileShader(shaderSrc.data(), getGlShaderType(shaderType));
    }

    for (shaderHandle const shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glAttachShader(m_shaderHandle, shaderHandle);
        }
    }

    glLinkProgram(m_shaderHandle);
    checkCompilerErrors(m_shaderHandle, GL_PROGRAM);

    for (shaderHandle const shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glDeleteShader(shaderHandle);
        }
    }
    bindUniformBufferBlock();
}

OpenGlShader::OpenGlShader(std::string name, std::unordered_map<ShaderType, std::string> const& shaderSrcs)
    : m_shaderHandle(glCreateProgram()), m_name(std::move(name)) {
    std::array<shaderHandle, k_shaderTypeCount> shaderHandles = {};
    for (auto const& [shaderType, shaderSrc] : shaderSrcs) {
        auto const index     = static_cast<size_t>(shaderType);
        shaderHandles[index] = compileShader(shaderSrc.data(), getGlShaderType(shaderType));
    }

    for (shaderHandle const shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glAttachShader(m_shaderHandle, shaderHandle);
        }
    }

    glLinkProgram(m_shaderHandle);
    checkCompilerErrors(m_shaderHandle, GL_PROGRAM);

    for (shaderHandle const shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glDeleteShader(shaderHandle);
        }
    }
    bindUniformBufferBlock();
}

void OpenGlShader::use() const {
    glUseProgram(m_shaderHandle);
}

void OpenGlShader::unBind() const {
    glUseProgram(0);
}

shaderHandle OpenGlShader::compileShader(char const* shaderCode, GLenum shaderType) {
    shaderHandle const shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderCode, nullptr);
    glCompileShader(shaderHandle);
    checkCompilerErrors(shaderHandle, shaderType);
    return shaderHandle;
}

GLint OpenGlShader::getUniformLocation(std::string const& name) const {
    if (m_uniformLocationCache.contains(name)) {
        return m_uniformLocationCache[name];
    }

    GLint const location         = glGetUniformLocation(m_shaderHandle, name.c_str());
    m_uniformLocationCache[name] = location;
    return location;
}

GLint OpenGlShader::getUniformBufferBlockIndex(std::string const& name) const {
    if (m_uniformLocationCache.contains(name)) {
        return m_uniformLocationCache[name];
    }

    GLint const blockIndex = glGetUniformBlockIndex(m_shaderHandle, name.data());
    LY_CORE_ASSERT(blockIndex >= 0, "Uniform buffer block is not yet initialized!");
    m_uniformLocationCache[name] = blockIndex;
    return blockIndex;
}

void OpenGlShader::bindUniformBufferBlock() const {
    OpenGlShader::use();
    setUniformBlock("Camera", UniformBufferBlockBinding::Camera);
    setUniformBlock("Scene", UniformBufferBlockBinding::Scene);
    setUniformBlock("Material", UniformBufferBlockBinding::Material);
    setUniformBlock("Object", UniformBufferBlockBinding::Object);
    OpenGlShader::unBind();
}

void OpenGlShader::checkCompilerErrors(shaderHandle shaderHandle, GLenum shaderType) {
    constexpr int bufferSize = 1024;
    GLint success            = 0;
    std::array<GLchar, bufferSize> infoLog{};

    if (shaderType == GL_PROGRAM) {
        glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
        if (success != 0) {
            return;
        }

        glGetProgramInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CORE_LOG(LogType::Error, "ERROR::PROGRAM_LINKING_ERROR ({})", infoLog.data());
    } else {
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
        if (success != 0) {
            return;
        }

        glGetShaderInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CORE_LOG(LogType::Error, "ERROR::SHADER_COMPILATION_ERROR ({})", infoLog.data());
    }
}

}  // namespace ly::renderer