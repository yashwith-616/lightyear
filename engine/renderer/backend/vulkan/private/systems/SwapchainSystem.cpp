#include "SwapchainSystem.h"

#include <any>
#include <components/SwapchainData.h>
#include <device/PhysicalDevice.h>

#include <vulkan/vulkan_raii.hpp>

#include "device/LogicalDevice.h"

namespace
{

vk::SurfaceFormatKHR selectSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& availableSurfaceFormats)
{
    assert(!availableSurfaceFormats.empty() && "Physical device does not have any surface formats");

    // eB8G8R8A8Srgb - may not be available on some older mobile chips
    auto it = std::ranges::find_if(availableSurfaceFormats, [](auto const& surfaceFormat) {
        return surfaceFormat.format == vk::Format::eB8G8R8A8Srgb &&
            surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
    });

    assert(it != availableSurfaceFormats.end() && "Required surface format was not enabled");
    return *it;
}

vk::PresentModeKHR selectPresentMode(std::vector<vk::PresentModeKHR> const& availablePresentModes)
{
    assert(!availablePresentModes.empty() && "Physical device does not have any present mode");

    bool isMailboxModePresent = std::ranges::any_of(
        availablePresentModes, [](auto const& presentMode) { return presentMode == vk::PresentModeKHR::eMailbox; });
    if (isMailboxModePresent)
    {
        return vk::PresentModeKHR::eMailbox;
    }

    auto checkForFifoMode = [](auto const& presentMode) { return presentMode == vk::PresentModeKHR::eFifo; };
    assert(std::ranges::any_of(availablePresentModes, checkForFifoMode) && "Fifo present mode KHR is not present");
    return vk::PresentModeKHR::eFifo;
}

uint32_t selectMinImageCount(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities)
{
    auto minImageCount = std::max<uint32_t>(3u, surfaceCapabilities.minImageCount);
    // maxImageCount is 0 if there is no limit for number of images to be created.
    if (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount)
    {
        minImageCount = std::min<uint32_t>(minImageCount, surfaceCapabilities.maxImageCount);
    }
    return minImageCount;
}

vk::Extent2D resolveExtent(vk::SurfaceCapabilitiesKHR const& surfaceCapabilities, ly::renderer::ImageExtent imgExtent)
{
    if (surfaceCapabilities.currentExtent.width != 0xFFFFFFFF)
    {
        return surfaceCapabilities.currentExtent;
    }

    // maxImageExtent : Can be 0 when window is minimized
    uint32_t width =
        std::clamp(imgExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
    uint32_t height = std::clamp(
        imgExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

    return vk::Extent2D{.width = width, .height = height};
}

vk::raii::SwapchainKHR createSwapchain(
    vk::raii::Device const& device,
    ly::renderer::Surface const& surface,
    vk::Extent2D imgExtent,
    std::optional<std::reference_wrapper<ly::renderer::SwapchainData const>> prevData,
    vk::SurfaceTransformFlagBitsKHR surfaceTransformFlag,
    uint32_t minImageCount,
    vk::SurfaceFormatKHR surfaceFormat,
    vk::PresentModeKHR presentMode)
{
    vk::SwapchainKHR oldHandle = prevData.has_value() ? *prevData->get().swapchain : nullptr;

    vk::SwapchainCreateInfoKHR swapchainCreateInfo{
        .flags = vk::SwapchainCreateFlagsKHR(),
        .surface = surface.getHandle(),
        .minImageCount = minImageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = imgExtent,
        .imageArrayLayers = 1,
        .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
        .imageSharingMode = vk::SharingMode::eExclusive,
        .preTransform = surfaceTransformFlag,
        .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
        .presentMode = presentMode,
        .clipped = true,
        .oldSwapchain = oldHandle};

    auto expected = device.createSwapchainKHR(swapchainCreateInfo);
    assert(expected.has_value() && "Swapchain has not been created");
    return std::move(expected.value());
}

} // namespace

namespace ly::renderer
{

struct SwapchainSystem::SwapchainCache
{
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    uint32_t minImageCount{0};

    SwapchainCache copy()
    {
        SwapchainCache cache;
        cache.surfaceFormat = surfaceFormat;
        cache.minImageCount = minImageCount;
        cache.presentMode = presentMode;
        return cache;
    }
};

SwapchainSystem::SwapchainSystem(
    PhysicalDevice const& physicalDevice, LogicalDevice const& device, Surface const& surface) :
    m_physicalDevice(physicalDevice), m_device(device), m_surface(surface)
{
    cacheCapabilities();
}

SwapchainSystem::~SwapchainSystem() = default;

SwapchainData
    SwapchainSystem::create(ImageExtent extent, std::optional<std::reference_wrapper<SwapchainData const>> prevData)
{
    SwapchainCache cacheCopy{};
    {
        std::shared_lock<std::shared_mutex> lock(m_cacheMutex);
        cacheCopy = m_swapchainCache->copy();
    }

    // if post-processing is applied in different format then surface format cannot be used here
    auto createImageFactory = [&](vk::Image const& image) {
        vk::ImageViewCreateInfo viewInfo{
            .image = image,
            .viewType = vk::ImageViewType::e2D,
            .format = cacheCopy.surfaceFormat.format,
            .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        auto expected = m_device.getHandle().createImageView(viewInfo);
        assert(expected.has_value() && "Swapchain ImageView has not been created");
        return std::move(expected.value());
    };

    // Create and populate surface capabilities
    vk::SurfaceCapabilitiesKHR const& surfaceCapabilities = m_physicalDevice.getSurfaceCapabilities();
    vk::Extent2D imgExtent = resolveExtent(surfaceCapabilities, extent);
    vk::raii::SwapchainKHR swapchain = createSwapchain(
        m_device.getHandle(),
        m_surface,
        imgExtent,
        prevData,
        surfaceCapabilities.currentTransform,
        cacheCopy.minImageCount,
        cacheCopy.surfaceFormat,
        cacheCopy.presentMode);

    SwapchainData data{
        .swapchain = std::move(swapchain),
        .extent = imgExtent,
        .colorFormat = cacheCopy.surfaceFormat.format,
        .sampleCountFlagBits = vk::SampleCountFlagBits::e1};

    data.images = data.swapchain.getImages();
    data.imageViews.reserve(data.images.size());
    std::ranges::transform(data.images, std::back_inserter(data.imageViews), createImageFactory);
    return data;
}

void SwapchainSystem::refreshCapabilities() { cacheCapabilities(); }

vk::SurfaceFormatKHR const& SwapchainSystem::getSurfaceFormat() const { return m_swapchainCache->surfaceFormat; }

/**
 * Cache hardware capabilities. Avoid unnecessary querying of hardware for swapchain re-creation. If
 * device properties are changed such as on multi-monitor setups then the refreshCapabilities function
 * must be called to refresh all the cached values
 */
void SwapchainSystem::cacheCapabilities()
{
    auto newCache = std::make_unique<SwapchainSystem::SwapchainCache>();
    newCache->surfaceFormat = selectSurfaceFormat(m_physicalDevice.getSurfaceFormatsKhr());
    newCache->presentMode = selectPresentMode(m_physicalDevice.getPresentModesKhr());
    newCache->minImageCount = selectMinImageCount(m_physicalDevice.getSurfaceCapabilities());

    {
        std::lock_guard<std::shared_mutex> lock(m_cacheMutex);
        m_swapchainCache = std::move(newCache);
    }
}

} // namespace ly::renderer
