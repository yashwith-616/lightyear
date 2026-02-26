#pragma once

#include <filesystem>
#include "device/LogicalDevice.h"

namespace ly::renderer
{

class Shader
{
public:
    Shader(LogicalDevice const& device, std::vector<std::byte> const& shaderCode);
    Shader(LogicalDevice const& device, std::filesystem::path const& shaderPath);

    vk::PipelineShaderStageCreateInfo
        createSubShader(vk::ShaderStageFlagBits type, std::string_view entrypointFuncName);

private:
    vk::raii::ShaderModule m_shaderModule{nullptr};
};

} // namespace ly::renderer
