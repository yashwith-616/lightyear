#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {

class LIGHTYEAR_API Shader {
public:
    virtual ~Shader()           = default;
    virtual void Use() const    = 0;
    virtual void UnBind() const = 0;

    virtual CName GetName() const { return m_Name; }

    static ly::Ref<Shader> Create(const CName& name,
                                  const std::unordered_map<ShaderType, CPath>& shaderFiles);

    static ly::Ref<Shader> Create(const CName& name,
                                  const std::unordered_map<ShaderType, CText>& shaderSrcs);

protected:
    CName m_Name{};

protected:
    /**
     * @brief Read file from path.
     * @param shaderFilePath the shader file path
     * @return string with the file data
     */
    virtual CText ReadFile(CPath shaderFilePath);
};

}  // namespace ly::renderer
