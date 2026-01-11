#include "Lightyear/Renderer/MaterialSystem/ShaderLibrary.h"

namespace ly::renderer
{

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    const auto& name = shader->GetName();

    LY_CORE_ASSERT(!IsPresent(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

Ref<Shader>
    ShaderLibrary::LoadShader(const std::string& shaderName, const std::unordered_map<ShaderType, CPath>& filePaths)
{
    auto shader = Shader::Create(shaderName, filePaths);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& shaderName) const
{
    const auto& iterator = m_Shaders.find(shaderName);
    LY_CORE_ASSERT(iterator != m_Shaders.end(), "Shader not found!");
    return iterator->second;
}

} // namespace ly::renderer
