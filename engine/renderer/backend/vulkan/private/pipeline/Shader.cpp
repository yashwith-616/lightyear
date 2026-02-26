#include "Shader.h"

namespace ly::renderer
{

Shader::Shader(LogicalDevice const& device, std::vector<std::byte> const& shaderCode)
{
    vk::ShaderModuleCreateInfo shaderModuleCreateInfo{
        .codeSize = shaderCode.size() * sizeof(std::byte),
        .pCode = reinterpret_cast<uint32_t const*>(shaderCode.data()),
    };

    auto shaderModuleExpect = device.getHandle().createShaderModule(shaderModuleCreateInfo);
    assert(shaderModuleExpect.has_value() && "Shader module could not be created");
    m_shaderModule = std::move(shaderModuleExpect.value());
}

vk::PipelineShaderStageCreateInfo const&
    Shader::createSubShader(vk::ShaderStageFlagBits type, std::string_view entrypointFuncName)
{
    return vk::PipelineShaderStageCreateInfo{
        .stage = type, .module = m_shaderModule, .pName = entrypointFuncName.data()};
}

} // namespace ly::renderer
