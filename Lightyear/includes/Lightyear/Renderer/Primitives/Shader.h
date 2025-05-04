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

    static ly::Ref<Shader> Create(CName name, std::unordered_map<ShaderType, CPath> shaderFiles);
    static ly::Ref<Shader> Create(CName name, std::unordered_map<ShaderType, CText> shaderSrcs);

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

class LIGHTYEAR_API ShaderLibrary {
public:
    virtual ~ShaderLibrary() = default;

    virtual void AddShader(Ref<Shader> shader) { m_ShaderLibrary[shader->GetName()] = shader; }

    virtual Ref<Shader> GetShader(CName shaderName) const {
        auto it = m_ShaderLibrary.find(shaderName);
        if (it != m_ShaderLibrary.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool IsPresent(CName shaderName) {
        return m_ShaderLibrary.find(shaderName) != m_ShaderLibrary.end();
    }

private:
    std::unordered_map<CName, Ref<Shader>> m_ShaderLibrary;
};

}  // namespace ly::renderer
