/**
 * @file PipelineBuilder.h
 * @author Yashwith
 * @brief
 * @date 2026-03-05
 */
#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace ly::renderer
{
enum class ColorBlendType : uint8_t
{
    Opaque,
    AlphaBlend
};

struct PipelineConfig;
class LogicalDevice;

class PipelineBuilder
{
public:
    PipelineBuilder(LogicalDevice const& device);
    virtual ~PipelineBuilder();

    PipelineBuilder& withTopology(vk::PrimitiveTopology topology);

    PipelineBuilder& withBlendMode(ColorBlendType type);

    PipelineBuilder& withCullMethod(vk::CullModeFlags mode);

    PipelineBuilder& withPolygonMode(vk::PolygonMode mode);

    PipelineBuilder& withDepthTest(bool enable, std::optional<vk::CompareOp> op);

    PipelineBuilder& withRasterizationDepthBias(
        float depthBiasConstantFactor,
        float depthBiasClamp,
        float depthBiasSlopeFactor);

    PipelineBuilder& withMultisampleCount(vk::SampleCountFlagBits sampleCount);

    PipelineBuilder& withLayout(vk::PipelineLayoutCreateInfo layoutInfo);

    PipelineBuilder& withAlphaToCoverageEnable(bool enable);

    vk::raii::Pipeline build(
        std::vector<vk::PipelineShaderStageCreateInfo> const& stages,
        vk::PipelineVertexInputStateCreateInfo vertexInputState,
        vk::PipelineLayoutCreateInfo layoutInfo,
        vk::Format colorFormat);

private:
    LogicalDevice const& m_device;
    std::unique_ptr<PipelineConfig> m_config;
};
} // namespace ly::renderer