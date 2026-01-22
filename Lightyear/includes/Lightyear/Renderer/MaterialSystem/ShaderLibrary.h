#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/Shader.h"

namespace ly::renderer
{

class LIGHTYEAR_API ShaderLibrary
{
public:
    virtual ~ShaderLibrary() = default;

    virtual void add(ref<Shader> const& shader);
    virtual ref<Shader> loadShader(
        std::string const& shaderName, std::unordered_map<ShaderType, std::filesystem::path> const& filePaths);

    virtual ref<Shader> get(std::string const& shaderName) const;

    bool isPresent(std::string const& shaderName) const { return m_shaders.contains(shaderName); }

private:
    std::unordered_map<std::string, ref<Shader>> m_shaders;
};

} // namespace ly::renderer
