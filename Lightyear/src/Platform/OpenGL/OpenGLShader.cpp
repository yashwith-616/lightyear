#include "Lightyear/Platform/OpenGL/OpenGLShader.h"

namespace ly::renderer {

OpenGLShader::OpenGLShader(std::string_view vertexPath, std::string_view fragmentPath) {
    try {
        std::string vertexCode   = ReadFile(vertexPath);
        std::string fragmentCode = ReadFile(fragmentPath);

        ShaderHandle vertexShader   = CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        ShaderHandle fragmentShader = CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
        m_ShaderHandle              = LinkProgram(vertexShader, fragmentShader);
    } catch (const std::exception& e) {
        LY_CORE_LOG(LogType::Error, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
    }
}

void OpenGLShader::Use() const {
    glUseProgram(m_ShaderHandle);
}

ShaderHandle OpenGLShader::CompileShader(const char* shaderCode, GLenum shaderType) {
    ShaderHandle shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderCode, nullptr);
    glCompileShader(shaderHandle);
    CheckCompilerErrors(shaderHandle, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shaderHandle;
}

ShaderHandle OpenGLShader::LinkProgram(ShaderHandle vertexShader, ShaderHandle fragmentShader) {
    ShaderHandle programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    CheckCompilerErrors(programHandle, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programHandle;
}

void OpenGLShader::CheckCompilerErrors(ShaderHandle shaderHandle, std::string_view type) {
    GLint success = 0;
    std::array<GLchar, 1024> infoLog{};

    if (type == "PROGRAM") {
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