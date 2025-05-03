#pragma once

#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {

class LIGHTYEAR_API Shader {
public:
    virtual ~Shader()        = default;
    virtual void Use() const = 0;

    static ly::Ref<Shader> Create(std::string_view vertexPath, std::string_view fragmentPath);

protected:
    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    virtual std::string ReadFile(std::string_view shaderFilePath);
};

}  // namespace ly::renderer
