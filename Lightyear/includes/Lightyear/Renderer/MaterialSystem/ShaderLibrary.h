#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/Shader.h"

namespace ly::renderer
{

class LIGHTYEAR_API ShaderLibrary
{
public:
    virtual ~ShaderLibrary() = default;

    virtual void Add(const Ref<Shader>& shader);
    virtual Ref<Shader> LoadShader(
        const std::string& shaderName, const std::unordered_map<ShaderType, std::filesystem::path>& filePaths);

    virtual Ref<Shader> Get(const std::string& shaderName) const;

    bool IsPresent(const std::string& shaderName) const { return m_Shaders.contains(shaderName); }

private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

} // namespace ly::renderer
