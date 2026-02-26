#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "device/LogicalDevice.h"

namespace ly::renderer
{

struct PipelineCreateInfo
{};


class Pipeline
{
public:
    Pipeline(LogicalDevice const& device);

    void createPipeline(
        std::vector<vk::PipelineShaderStageCreateInfo> const& stages,
        vk::Extent2D extent,
        vk::SurfaceFormatKHR const& surface);

    vk::raii::Pipeline const& getPipeline() const { return m_pipeline; }

private:
    std::vector<vk::DynamicState> configureDynamicState();
    vk::Viewport configureViewport(vk::Extent2D extent);

    vk::PipelineRasterizationStateCreateInfo configureRasterization();
    vk::PipelineMultisampleStateCreateInfo configureMultisample();

    vk::PipelineColorBlendStateCreateInfo configureColorBlendState();
    vk::PipelineColorBlendAttachmentState configureColorBlendAttachment();

    vk::PipelineInputAssemblyStateCreateInfo configureInputAssemblyState();
    vk::PipelineVertexInputStateCreateInfo configureVertexInputState();

    LogicalDevice const& m_device;
    vk::raii::Pipeline m_pipeline{nullptr};
};

} // namespace ly::renderer
