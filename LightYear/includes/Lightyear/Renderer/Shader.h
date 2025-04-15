#pragma once

#include "Lightyear/LightyearAPI.h"
#include <glad.h>
#include <glm/glm.hpp>
#include <lypch.h>

namespace ly {

class LIGHTYEAR_API Shader {
public:
    unsigned int id_;

    Shader(std::string_view vertexPath, std::string_view fragmantPath);

    void use() const;

    template <typename T>
        requires(!std::is_arithmetic_v<T>)
    void setUniform(std::string_view name, const T& value) const
    {
        static_assert(sizeof(T) == 0, "Unsupported uniform type! Specialize setUniform<T> for this type");
    }

    // Specialization for int
    template <std::integral T>
    inline void setUniform(std::string_view name, const T value) const
    {
        glUniform1i(glGetUniformLocation(id_, name.data()), static_cast<int>(value));
    }

    // Specialization for float
    template <std::floating_point T>
    inline void setUniform(std::string_view name, const T value) const
    {
        glUniform1f(glGetUniformLocation(id_, name.data()), static_cast<float>(value));
    }

    // Specialization for bool
    template <std::same_as<bool> T>
    inline void setUniform(std::string_view name, const T value) const
    {
        glUniform1i(glGetUniformLocation(id_, name.data()), static_cast<bool>(value));
    }

    // Specialization for glm::vec2
    template <>
    inline void setUniform<glm::vec2>(std::string_view name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }

    // Specialization for glm::vec3
    template <>
    inline void setUniform<glm::vec3>(std::string_view name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }

    // Specialization for glm::vec4
    template <>
    inline void setUniform<glm::vec4>(std::string_view name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id_, name.data()), 1, &value[0]);
    }

    // Specialization for glm::mat2
    template <>
    inline void setUniform<glm::mat2>(std::string_view name, const glm::mat2& value) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &value[0][0]);
    }

    // Specialization for glm::mat3
    template <>
    inline void setUniform<glm::mat3>(std::string_view name, const glm::mat3& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &value[0][0]);
    }

    // Specialization for glm::mat4
    template <>
    inline void setUniform<glm::mat4>(std::string_view name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, &value[0][0]);
    }

private:
    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    std::string readFile(std::string_view shaderFilePath);

    /**
     * @brief Compile shader code
     * @param shaderCode the shader code
     * @param shaderType the shader type
     * @return The shader handle
     */
    unsigned int compileShader(const char* shaderCode, GLenum shaderType);

    /**
     * @brief Link shader
     * @param vertexShader the vertex shader
     * @param fragmentShader the fragment shader
     * @return the shader program handle
     */
    unsigned int linkProgram(GLuint vertexShader, GLuint fragmentShader);

    /**
     * @brief Check if there is any compiler errors while compiling shader
     * @param shaderHandle the shader handle that needs to be compiled
     * @param type the shader type
     */
    void checkCompilerErrors(GLuint shaderHandle, std::string_view type);
};

}
