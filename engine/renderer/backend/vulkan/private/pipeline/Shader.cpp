#include "Shader.h"

#include <fstream>

namespace
{

std::vector<std::byte> readFile(std::filesystem::path const& filePath)
{
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);
    assert(file.is_open() && "failed to open file");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<std::byte> buffer(fileSize);

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    return buffer;
}

} // namespace

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

Shader::Shader(LogicalDevice const& device, std::filesystem::path const& shaderPath) :
    Shader(device, readFile(shaderPath))
{}

vk::PipelineShaderStageCreateInfo
    Shader::createSubShader(vk::ShaderStageFlagBits type, std::string_view entrypointFuncName)
{
    return vk::PipelineShaderStageCreateInfo{
        .stage = type, .module = *m_shaderModule, .pName = entrypointFuncName.data()};
}

} // namespace ly::renderer
