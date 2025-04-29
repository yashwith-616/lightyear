#pragma once

#include <glad.h>
#include <glm/glm.hpp>
#include "Lightyear/Renderer/Primitives/Shader.h"

namespace ly::renderer {

class OpenGLShader : public Shader {
public:
    OpenGLShader(std::string_view vertexPath, std::string_view fragmentPath);

    virtual void Use() const override;

private:
    ShaderHandle m_ShaderHandle;

private:
    /**
     * @brief Compile shader code
     * @param shaderCode the shader code
     * @param shaderType the shader type
     * @return The shader handle
     */
    ShaderHandle CompileShader(const char* shaderCode, GLenum shaderType);

    /**
     * @brief Link shader
     * @param vertexShader the vertex shader
     * @param fragmentShader the fragment shader
     * @return the shader program handle
     */
    ShaderHandle LinkProgram(ShaderHandle vertexShader, ShaderHandle fragmentShader);

    /**
     * @brief Check if there is any compiler errors while compiling shader
     * @param shaderHandle the shader handle that needs to be compiled
     * @param type the shader type
     */
    void CheckCompilerErrors(ShaderHandle shaderHandle, std::string_view type);
};

}  // namespace ly::renderer