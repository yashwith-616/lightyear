#include "Lightyear/Renderer/MaterialSystem/ShaderLibrary.h"

namespace ly::renderer {

void ShaderLibrary::Add(Ref<Shader> shader) {
    const auto& name = shader->GetName();

    LY_CORE_ASSERT(!IsPresent(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::LoadShader(const CName& shaderName,
                                      const std::unordered_map<ShaderType, CPath>& filePaths) {
    auto shader = Shader::Create(shaderName, filePaths);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const CName& shaderName) const {
    const auto& it = m_Shaders.find(shaderName);
    LY_CORE_ASSERT(it != m_Shaders.end(), "Shader not found!");
    return it->second;
}

}  // namespace ly::renderer