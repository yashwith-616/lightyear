#include "Lightyear/Platform/OpenGL/OpenGLShader.h"

namespace ly::renderer {

OpenGLShader::OpenGLShader(std::string name,
                           const std::unordered_map<ShaderType, CPath>& shaderFiles)
    : m_Name(std::move(name)) {
    std::array<ShaderHandle, ShaderTypeCount> shaderHandles = {};
    for (const auto& [shaderType, path] : shaderFiles) {
        CText shaderSrc = ReadFile(path);
        const ShaderHandle shaderHandle =
            CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
        const auto index     = static_cast<size_t>(shaderType);
        shaderHandles[index] = CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
    }

    m_ShaderHandle = glCreateProgram();
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

OpenGLShader::OpenGLShader(std::string name,
                           const std::unordered_map<ShaderType, CText>& shaderSrcs)
    : m_Name(std::move(name)) {
    std::array<ShaderHandle, ShaderTypeCount> shaderHandles = {};
    for (const auto& [shaderType, shaderSrc] : shaderSrcs) {
        const ShaderHandle shaderHandle =
            CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
        const auto index     = static_cast<size_t>(shaderType);
        shaderHandles[index] = CompileShader(shaderSrc.data(), GetGLShaderType(shaderType));
    }

    m_ShaderHandle = glCreateProgram();
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
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    const GLint location         = glGetUniformLocation(m_ShaderHandle, name.c_str());
    m_UniformLocationCache[name] = location;
    return location;
}

GLint OpenGLShader::GetUniformBufferBlockIndex(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    const GLint blockIndex = glGetUniformBlockIndex(m_ShaderHandle, name.data());
    LY_CORE_ASSERT(blockIndex >= 0, "Uniform buffer block is not yet initialized!");
    m_UniformLocationCache[name] = blockIndex;
    return blockIndex;
}

void OpenGLShader::BindUniformBufferBlock() const {
    Use();
    SetUniformBlock("Camera", UniformBufferBlockBinding::Camera);
    SetUniformBlock("Scene", UniformBufferBlockBinding::Scene);
    SetUniformBlock("Material", UniformBufferBlockBinding::Material);
    SetUniformBlock("Object", UniformBufferBlockBinding::Object);
    UnBind();
}

GLenum OpenGLShader::GetGLShaderType(ShaderType shaderType) {
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
    }
}

void OpenGLShader::CheckCompilerErrors(ShaderHandle shaderHandle, GLenum shaderType) {
    GLint success = 0;
    std::array<GLchar, 1024> infoLog{};

    if (shaderType == GL_PROGRAM) {
        glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
        if (success) return;

        glGetProgramInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CORE_LOG(LogType::Error, "ERROR::PROGRAM_LINKING_ERROR ({})", infoLog.data());
    } else {
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
        if (success) return;

        glGetShaderInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CORE_LOG(LogType::Error, "ERROR::SHADER_COMPILATION_ERROR ({})", infoLog.data());
    }
}

}  // namespace ly::renderer