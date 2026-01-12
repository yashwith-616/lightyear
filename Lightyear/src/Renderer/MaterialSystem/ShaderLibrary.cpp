#include "Lightyear/Renderer/MaterialSystem/ShaderLibrary.h"

namespace ly::renderer {

void ShaderLibrary::add(ref<Shader> const& shader) {
    auto const& name = shader->getName();

    LY_CORE_ASSERT(!isPresent(name), "Shader already exists!");
    m_shaders[name] = shader;
}

ref<Shader> ShaderLibrary::loadShader(std::string const& shaderName,
                                      std::unordered_map<ShaderType, cPath> const& filePaths) {
    auto shader = Shader::create(shaderName, filePaths);
    add(shader);
    return shader;
}

ref<Shader> ShaderLibrary::get(std::string const& shaderName) const {
    auto const& iterator = m_shaders.find(shaderName);
    LY_CORE_ASSERT(iterator != m_shaders.end(), "Shader not found!");
    return iterator->second;
}

}  // namespace ly::renderer