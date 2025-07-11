#pragma once

#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {

class LIGHTYEAR_API Shader {
public:
    virtual ~Shader()           = default;
    virtual void Use() const    = 0;
    virtual void UnBind() const = 0;

    virtual std::string GetName() const { return m_Name; }

    static ly::Ref<Shader> Create(const std::string& name,
                                  const std::unordered_map<ShaderType, std::filesystem::path>& shaderFiles);

    static ly::Ref<Shader> Create(const std::string& name,
                                  const std::unordered_map<ShaderType, std::string>& shaderSrcs);

protected:
    std::string m_Name{};

protected:
    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    virtual std::string ReadFile(std::filesystem::path shaderFilePath);
};

}  // namespace ly::renderer
