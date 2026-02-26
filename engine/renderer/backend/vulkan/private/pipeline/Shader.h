#pragma once
#include "device/LogicalDevice.h"

namespace ly::renderer
{

class Shader
{
public:
    Shader(LogicalDevice const& device, std::vector<std::byte> const& shaderCode);

    vk::PipelineShaderStageCreateInfo const&
        createSubShader(vk::ShaderStageFlagBits type, std::string_view entrypointFuncName);

private:
    vk::ShaderModule m_shaderModule;
};

} // namespace ly::renderer
