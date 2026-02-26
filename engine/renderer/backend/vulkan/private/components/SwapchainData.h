#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace ly::renderer
{

// Could use dirty flags and reset the swapchain and start of the renderer
struct SwapchainData
{
    vk::raii::SwapchainKHR swapchain{nullptr};
    vk::Extent2D extent;

    // color
    std::vector<vk::Image> images;
    std::vector<vk::ImageView> imageViews;
    vk::Format colorFormat{};

    // depth + stencil
    std::vector<vk::Image> depthImages;
    std::vector<vk::ImageView> depthImageViews;
    vk::Format depthFormat{};

    // MSAA
    vk::SampleCountFlagBits sampleCountFlagBits{vk::SampleCountFlagBits::e1};
};

} // namespace ly::renderer
