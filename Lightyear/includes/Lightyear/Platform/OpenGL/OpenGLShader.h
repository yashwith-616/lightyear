#pragma once

#include <glm/glm.hpp>
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "glad.h"

namespace ly::renderer {

class OpenGLShader : public Shader {
public:
    /**
     * @brief Create OpenGL shaders from file.
     * @param name The compiled shader name
     * @param shaderFiles The ShaderType along with the path to file
     */
    OpenGLShader(const CName& name, const std::unordered_map<ShaderType, CPath>& shaderFiles);

    /**
     * @brief Create OpenGL shaders from source code
     * @param name The compiled shader name
     * @param shaderSrcs The ShaderType along with it's source code
     */
    OpenGLShader(const CName& name, const std::unordered_map<ShaderType, CText>& shaderSrcs);

    virtual void Use() const override;

    virtual void UnBind() const override;

    template <typename T>
        requires(!std::is_arithmetic_v<T>)
    void SetUniform(CParam name, const T& value) const {
        static_assert(sizeof(T) == 0,
                      "Unsupported uniform type! Specialize setUniform<T> for this type");
    }

    // Specialization for int
    template <std::integral T>
    inline void SetUniform(CParam name, const T value) const {
        glUniform1i(glGetUniformLocation(m_ShaderHandle, name.data()), static_cast<int>(value));
    }

    // Specialization for float
    template <std::floating_point T>
    inline void SetUniform(CParam name, const T value) const {
        glUniform1f(glGetUniformLocation(m_ShaderHandle, name.data()), static_cast<float>(value));
    }

    // Specialization for bool
    template <std::same_as<bool> T>
    inline void SetUniform(CParam name, const T value) const {
        glUniform1i(glGetUniformLocation(m_ShaderHandle, name.data()), static_cast<bool>(value));
    }

    // Specialization for glm::vec2
    template <>
    inline void SetUniform<glm::vec2>(CParam name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_ShaderHandle, name.data()), 1, &value[0]);
    }

    // Specialization for glm::vec3
    template <>
    inline void SetUniform<glm::vec3>(CParam name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_ShaderHandle, name.data()), 1, &value[0]);
    }

    // Specialization for glm::vec4
    template <>
    inline void SetUniform<glm::vec4>(CParam name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_ShaderHandle, name.data()), 1, &value[0]);
    }

    // Specialization for glm::mat2
    template <>
    inline void SetUniform<glm::mat2>(CParam name, const glm::mat2& value) const {
        glUniformMatrix2fv(
            glGetUniformLocation(m_ShaderHandle, name.data()), 1, GL_FALSE, &value[0][0]);
    }

    // Specialization for glm::mat3
    template <>
    inline void SetUniform<glm::mat3>(CParam name, const glm::mat3& value) const {
        glUniformMatrix3fv(
            glGetUniformLocation(m_ShaderHandle, name.data()), 1, GL_FALSE, &value[0][0]);
    }

    // Specialization for glm::mat4
    template <>
    inline void SetUniform<glm::mat4>(CParam name, const glm::mat4& value) const {
        glUniformMatrix4fv(
            glGetUniformLocation(m_ShaderHandle, name.data()), 1, GL_FALSE, &value[0][0]);
    }

private:
    ShaderHandle m_ShaderHandle;
    CText m_Name;

private:
    /**
     * @brief Compile shader code
     * @param shaderCode the shader code
     * @param shaderType the shader type
     * @return The shader handle
     */
    ShaderHandle CompileShader(const char* shaderCode, GLenum shaderType);

    /**
     * @brief Check if there is any compiler errors while compiling shader
     * @param shaderHandle the shader handle that needs to be compiled
     * @param type the shader type
     */
    void CheckCompilerErrors(ShaderHandle shaderHandle, GLenum shaderType);

    GLenum GetGLShaderType(ShaderType shaderType) const;
};

}  // namespace ly::renderer