#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/Shader.h"

namespace ly::renderer {

class LIGHTYEAR_API ShaderLibrary {
public:
    virtual ~ShaderLibrary() = default;

    virtual void Add(Ref<Shader> shader);
    virtual Ref<Shader> LoadShader(const CName& shaderName, const std::unordered_map<ShaderType, CPath>& filePaths);

    virtual Ref<Shader> Get(const CName& shaderName) const;

    bool IsPresent(const CName& shaderName) const { return m_Shaders.contains(shaderName); }

private:
    std::unordered_map<CName, Ref<Shader>> m_Shaders;
};

}  // namespace ly::renderer