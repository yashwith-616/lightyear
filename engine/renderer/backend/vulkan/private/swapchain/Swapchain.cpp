#include "Swapchain.h"

namespace ly::renderer
{

Swapchain::Swapchain(
    PhysicalDevice const& physicalDevice, LogicalDevice const& device, vk::SurfaceKHR surface, int width, int height) :
    m_physicalDevice(physicalDevice), m_device(device)
{
    auto const& surfaceCapabilities = m_physicalDevice.getSurfaceCapabilities().surfaceCapabilities;

    m_surfaceFormat = chooseSurfaceFormat();
    m_presentMode = choosePresentMode();
    m_extent = chooseExtent(surfaceCapabilities, width, height);

    auto minImageCount = std::max<uint32_t>(3u, surfaceCapabilities.minImageCount);
    if (surfaceCapabilities.maxImageCount > 0)
    {
        minImageCount = std::min<uint32_t>(minImageCount, surfaceCapabilities.maxImageCount);
    }

    vk::SwapchainCreateInfoKHR swapChainCreateInfo{
        .flags = vk::SwapchainCreateFlagsKHR(),
        .surface = std::move(surface),
        .minImageCount = minImageCount,
        .imageFormat = m_surfaceFormat.format,
        .imageColorSpace = m_surfaceFormat.colorSpace,
        .imageExtent = m_extent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = m_presentMode,
        .clipped = true,
        .oldSwapchain = nullptr};

    m_swapchain = vk::raii::SwapchainKHR(m_device.getHandle(), swapChainCreateInfo);
}

vk::SurfaceFormatKHR Swapchain::chooseSurfaceFormat()
{
    std::vector<vk::SurfaceFormatKHR> availableFormats = m_physicalDevice.getSurfaceFormatsKhr();
    assert(!availableFormats.empty() && "Available surface formats are not available");

    auto it = std::ranges::find_if(availableFormats, [](auto const& surfaceFmt) {
        return surfaceFmt.format == vk::Format::eB8G8R8A8Srgb &&
            surfaceFmt.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
    });

    assert(it != availableFormats.end() && "Required surface format was not available");
    return *it;
}

vk::PresentModeKHR Swapchain::choosePresentMode()
{
    std::vector<vk::PresentModeKHR> availablePresentModes = m_physicalDevice.getPresentModesKhr();
    assert(!availablePresentModes.empty() && "Available present modes are not available");

    bool isMailboxModePresent = std::ranges::any_of(
        availablePresentModes, [](auto const& presentMode) { return presentMode == vk::PresentModeKHR::eMailbox; });
    if (isMailboxModePresent)
    {
        return vk::PresentModeKHR::eMailbox;
    }

    assert(
        std::ranges::any_of(
            availablePresentModes, [](auto const& presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }) &&
        "Fifo present mode KHR is not present");

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Swapchain::chooseExtent(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities, int width, int height)
{
    if (surfaceCapabilities.currentExtent.width != 0xFFFFFFFF)
    {
        return surfaceCapabilities.currentExtent;
    }

    return vk::Extent2D{
        .width = std::clamp<uint32_t>(
            width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
        .height = std::clamp<uint32_t>(
            height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)};
}

} // namespace ly::renderer
