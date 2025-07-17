#include "Lightyear/Platform/OpenGL/Renderer/Primitives/OpenGLShader.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace {
using ly::renderer::ShaderType;

GLenum GetGLShaderType(ShaderType shaderType) {
    switch (shaderType) {
        case ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderType::PIXEL:
        case ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case ShaderType::COMPUTE:
            return GL_COMPUTE_SHADER;
        case ShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case ShaderType::TESS_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TESS_EVALUATION:
            return GL_TESS_EVALUATION_SHADER;
        default:
            LY_CORE_ASSERT(false, "Invalid Shader Type!");
            return GL_INVALID_ENUM;
    }
}
}  // namespace

namespace ly::renderer {

OpenGLShader::OpenGLShader(std::string name, const std::unordered_map<ShaderType, CPath>& shaderFiles)
    : m_ShaderHandle(glCreateProgram()), m_Name(std::move(name)) {
    std::array<ShaderHandle, g_ShaderTypeCount> shaderHandles = {};
    for (const auto& [shaderType, path] : shaderFiles) {
        std::string shaderSrc = ReadFile(path);
        const auto index      = static_cast<size_t>(shaderType);
        shaderHandles[index]  = CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
    }

    for (const ShaderHandle shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glAttachShader(m_ShaderHandle, shaderHandle);
        }
    }

    glLinkProgram(m_ShaderHandle);
    CheckCompilerErrors(m_ShaderHandle, GL_PROGRAM);

    for (const ShaderHandle shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glDeleteShader(shaderHandle);
        }
    }
    BindUniformBufferBlock();
}

OpenGLShader::OpenGLShader(std::string name, const std::unordered_map<ShaderType, std::string>& shaderSrcs)
    : m_ShaderHandle(glCreateProgram()), m_Name(std::move(name)) {
    std::array<ShaderHandle, g_ShaderTypeCount> shaderHandles = {};
    for (const auto& [shaderType, shaderSrc] : shaderSrcs) {
        const auto index     = static_cast<size_t>(shaderType);
        shaderHandles[index] = CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
    }

    for (const ShaderHandle shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glAttachShader(m_ShaderHandle, shaderHandle);
        }
    }

    glLinkProgram(m_ShaderHandle);
    CheckCompilerErrors(m_ShaderHandle, GL_PROGRAM);

    for (const ShaderHandle shaderHandle : shaderHandles) {
        if (shaderHandle != 0) {
            glDeleteShader(shaderHandle);
        }
    }
    BindUniformBufferBlock();
}

void OpenGLShader::Use() const {
    glUseProgram(m_ShaderHandle);
}

void OpenGLShader::UnBind() const {
    glUseProgram(0);
}

ShaderHandle OpenGLShader::CompileShader(const char* shaderCode, GLenum shaderType) {
    const ShaderHandle shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderCode, nullptr);
    glCompileShader(shaderHandle);
    CheckCompilerErrors(shaderHandle, shaderType);
    return shaderHandle;
}

GLint OpenGLShader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.contains(name)) {
        return m_UniformLocationCache[name];
    }

    const GLint location         = glGetUniformLocation(m_ShaderHandle, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}

GLint OpenGLShader::GetUniformBufferBlockIndex(const std::string& name) const {
    if (m_UniformLocationCache.contains(name)) {
        return m_UniformLocationCache[name];
    }

    const GLint blockIndex = glGetUniformBlockIndex(m_ShaderHandle, name.data());
    LY_CORE_ASSERT(blockIndex >= 0, "Uniform buffer block is not yet initialized!");
    m_UniformLocationCache[name] = blockIndex;
    return blockIndex;
}

void OpenGLShader::BindUniformBufferBlock() const {
    OpenGLShader::Use();
    SetUniformBlock("Camera", UniformBufferBlockBinding::CAMERA);
    SetUniformBlock("Scene", UniformBufferBlockBinding::SCENE);
    SetUniformBlock("Material", UniformBufferBlockBinding::MATERIAL);
    SetUniformBlock("Object", UniformBufferBlockBinding::OBJECT);
    OpenGLShader::UnBind();
}

void OpenGLShader::CheckCompilerErrors(ShaderHandle shaderHandle, GLenum shaderType) {
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