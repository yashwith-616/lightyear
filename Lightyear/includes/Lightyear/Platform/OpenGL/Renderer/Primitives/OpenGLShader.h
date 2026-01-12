#pragma once

#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/Shader.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace ly::renderer {

class OpenGLShader : public Shader {
public:
    /**
     * @brief Create OpenGL shaders from file.
     * @param name The compiled shader name
     * @param shaderFiles The ShaderType along with the path to file
     */
    OpenGLShader(std::string name, const std::unordered_map<ShaderType, std::filesystem::path>& shaderFiles);

    /**
     * @brief Create OpenGL shaders from source code
     * @param name The compiled shader name
     * @param shaderSrcs The ShaderType along with its source code
     */
    OpenGLShader(std::string name, const std::unordered_map<ShaderType, std::string>& shaderSrcs);

    void Use() const override;

    void UnBind() const override;

    template <typename T>
        requires(!std::is_arithmetic_v<T>)
    void SetUniform(const std::string& /*name*/, const T& /*value*/) const {
        static_assert(sizeof(T) == 0, "Unsupported uniform type! Specialize setUniform<T> for this type");
    }

    // Overload for integer (int, long, etc.)
    template <std::integral T>
    void SetUniform(const std::string& name, const T value) const {
        glUniform1i(GetUniformLocation(name), static_cast<int>(value));
    }

    // Overload for floating types (float, double, etc.)
    template <std::floating_point T>
    void SetUniform(const std::string& name, const T value) const {
        glUniform1f(GetUniformLocation(name), static_cast<float>(value));
    }

    // Overload for boolean type
    template <std::same_as<bool> T>
    void SetUniform(const std::string& name, const T value) const {
        glUniform1i(GetUniformLocation(name), static_cast<bool>(value));
    }

    void SetUniformBlock(const std::string& name, const UniformBufferBlockBinding& bindingPoint) const {
        glUniformBlockBinding(m_ShaderHandle, GetUniformBufferBlockIndex(name), static_cast<uint32_t>(bindingPoint));
    }

    ShaderHandle GetShaderHandle() const { return m_ShaderHandle; }

private:
    ShaderHandle m_ShaderHandle{};
    std::string m_Name{ "NULL" };
    mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

    /**
     * @brief Compile shader code
     * @param shaderCode the shader code
     * @param shaderType the shader type
     * @return The shader handle
     */
    static ShaderHandle CompileShader(const char* shaderCode, GLenum shaderType);

    GLint GetUniformLocation(const std::string& name) const;

    GLint GetUniformBufferBlockIndex(const std::string& name) const;

    void BindUniformBufferBlock() const;

    /**
     * @brief Check if there is any compiler errors while compiling shader
     * @param shaderHandle the shader handle that needs to be compiled
     * @param shaderType the shader type
     */
    static void CheckCompilerErrors(ShaderHandle shaderHandle, GLenum shaderType);
};

#pragma region Template Specialization

// Specialization for glm::vec2
template <>
inline void OpenGLShader::SetUniform<glm::vec2>(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::vec3
template <>
inline void OpenGLShader::SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::vec4
template <>
inline void OpenGLShader::SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(GetUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::mat2
template <>
inline void OpenGLShader::SetUniform<glm::mat2>(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

// Specialization for glm::mat3
template <>
inline void OpenGLShader::SetUniform<glm::mat3>(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

// Specialization for glm::mat4
template <>
inline void OpenGLShader::SetUniform<glm::mat4>(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

#pragma endregion

}  // namespace ly::renderer