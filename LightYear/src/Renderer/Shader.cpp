#include "Lightyear/Renderer/Shader.h"
#include <Lightyear/Core/Log.h>
#include <glad.h>
#include <lypch.h>

namespace ly {

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath) : id_(0) {
    try {
        std::string vertexCode   = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        GLuint vertexShader   = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
        this->id_             = linkProgram(vertexShader, fragmentShader);
    } catch (const std::exception& e) {
        LY_CLIENT_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}", e.what());
    }
}

void Shader::use() const {
    glUseProgram(id_);
}

std::string Shader::readFile(std::string_view shaderFilePath) {
    // Open file in binary, and point the pointer to end of file.
    std::ifstream file(shaderFilePath.data(), std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error(std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}",
                                             std::string(shaderFilePath)));
    }

    // Get size of file by reading the current position of the file pointer(at the end)
    std::streamsize size = file.tellg();
    // Move the file pointer to the beginning and create a string with exact size
    file.seekg(0, std::ios::beg);
    std::string content(size, '\0');
    file.read(&content[0], size);
    return content;
}

unsigned int Shader::compileShader(const char* shaderCode, GLenum shaderType) {
    unsigned int shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &shaderCode, nullptr);
    glCompileShader(shaderHandle);
    checkCompilerErrors(shaderHandle, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shaderHandle;
}

unsigned int Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    unsigned int programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    checkCompilerErrors(programHandle, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programHandle;
}

void Shader::checkCompilerErrors(GLuint shaderHandle, std::string_view type) {
    GLint success = 0;
    std::array<GLchar, 1024> infoLog{};

    if (type == "PROGRAM") {
        glGetProgramiv(shaderHandle, GL_LINK_STATUS, &success);
        if (success) return;

        glGetProgramInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CLIENT_ERROR("ERROR::PROGRAM_LINKING_ERROR ({})", infoLog.data());
    } else {
        glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
        if (success) return;

        glGetShaderInfoLog(shaderHandle, infoLog.size(), nullptr, infoLog.data());
        LY_CLIENT_ERROR("ERROR::SHADER_COMPILATION_ERROR ({})", infoLog.data());
    }
}
}  // namespace ly
