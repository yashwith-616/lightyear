#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "../device/LogicalDevice.h"
#include "../device/PhysicalDevice.h"

namespace ly::renderer
{

class Swapchain
{
public:
    Swapchain(
        PhysicalDevice const& physicalDevice,
        LogicalDevice const& device,
        vk::SurfaceKHR surface,
        int width,
        int height);

private:
    vk::SurfaceFormatKHR chooseSurfaceFormat();
    vk::PresentModeKHR choosePresentMode();
    vk::Extent2D chooseExtent(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities, int width, int height);

    PhysicalDevice const& m_physicalDevice;
    LogicalDevice const& m_device;

    vk::raii::SwapchainKHR m_swapchain{nullptr};
    vk::SurfaceFormatKHR m_surfaceFormat{};
    vk::PresentModeKHR m_presentMode{};
    vk::Extent2D m_extent{};
};

} // namespace ly::renderer
