#pragma once

#include "Lightyear/Renderer/Primitives/RenderTypes.h"
#include "Lightyear/Renderer/Primitives/Shader.h"

LY_DISABLE_WARNINGS_PUSH
#include <glad/glad.h>
LY_DISABLE_WARNINGS_POP

namespace ly::renderer {

class OpenGlShader : public Shader {
public:
    /**
     * @brief Create OpenGL shaders from file.
     * @param name The compiled shader name
     * @param shaderFiles The ShaderType along with the path to file
     */
    OpenGlShader(std::string name, std::unordered_map<ShaderType, std::filesystem::path> const& shaderFiles);

    /**
     * @brief Create OpenGL shaders from source code
     * @param name The compiled shader name
     * @param shaderSrcs The ShaderType along with its source code
     */
    OpenGlShader(std::string name, std::unordered_map<ShaderType, std::string> const& shaderSrcs);

    void use() const override;

    void unBind() const override;

    template <typename T>
        requires(!std::is_arithmetic_v<T>)
    void setUniform(std::string const& /*name*/, T const& /*value*/) const {
        static_assert(sizeof(T) == 0, "Unsupported uniform type! Specialize setUniform<T> for this type");
    }

    // Overload for integer (int, long, etc.)
    template <std::integral T>
    void setUniform(std::string const& name, T const value) const {
        glUniform1i(getUniformLocation(name), static_cast<int>(value));
    }

    // Overload for floating types (float, double, etc.)
    template <std::floating_point T>
    void setUniform(std::string const& name, T const value) const {
        glUniform1f(getUniformLocation(name), static_cast<float>(value));
    }

    // Overload for boolean type
    template <std::same_as<bool> T>
    void setUniform(std::string const& name, T const value) const {
        glUniform1i(getUniformLocation(name), static_cast<bool>(value));
    }

    void setUniformBlock(std::string const& name, UniformBufferBlockBinding const& bindingPoint) const {
        glUniformBlockBinding(m_shaderHandle, getUniformBufferBlockIndex(name), static_cast<uint32_t>(bindingPoint));
    }

    shaderHandle getShaderHandle() const { return m_shaderHandle; }

private:
    shaderHandle m_shaderHandle{};
    std::string m_name{ "NULL" };
    mutable std::unordered_map<std::string, GLint> m_uniformLocationCache;

    /**
     * @brief Compile shader code
     * @param shaderCode the shader code
     * @param shaderType the shader type
     * @return The shader handle
     */
    static shaderHandle compileShader(char const* shaderCode, GLenum shaderType);

    GLint getUniformLocation(std::string const& name) const;

    GLint getUniformBufferBlockIndex(std::string const& name) const;

    void bindUniformBufferBlock() const;

    /**
     * @brief Check if there is any compiler errors while compiling shader
     * @param shaderHandle the shader handle that needs to be compiled
     * @param shaderType the shader type
     */
    static void checkCompilerErrors(shaderHandle shaderHandle, GLenum shaderType);
};

#pragma region Template Specialization

// Specialization for glm::vec2
template <>
inline void OpenGlShader::setUniform<glm::vec2>(std::string const& name, glm::vec2 const& value) const {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::vec3
template <>
inline void OpenGlShader::setUniform<glm::vec3>(std::string const& name, glm::vec3 const& value) const {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::vec4
template <>
inline void OpenGlShader::setUniform<glm::vec4>(std::string const& name, glm::vec4 const& value) const {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

// Specialization for glm::mat2
template <>
inline void OpenGlShader::setUniform<glm::mat2>(std::string const& name, glm::mat2 const& value) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

// Specialization for glm::mat3
template <>
inline void OpenGlShader::setUniform<glm::mat3>(std::string const& name, glm::mat3 const& value) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

// Specialization for glm::mat4
template <>
inline void OpenGlShader::setUniform<glm::mat4>(std::string const& name, glm::mat4 const& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

#pragma endregion

}  // namespace ly::renderer