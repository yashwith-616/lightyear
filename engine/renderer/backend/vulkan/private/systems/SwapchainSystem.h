#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <shared_mutex>

namespace vk
{
struct SurfaceFormatKHR;
}

namespace ly::renderer
{

class PhysicalDevice;
class LogicalDevice;
class Surface;
struct SwapchainData;

struct ImageExtent
{
    uint32_t height;
    uint32_t width;
};

class SwapchainSystem
{
public:
    SwapchainSystem(PhysicalDevice const& physicalDevice, LogicalDevice const& device, Surface const& surface);

    ~SwapchainSystem();

    /**
     * Create a swapchain image. Passing old data on recreation will try to reuse the existing resource to create
     * the new one.
     *
     * \param extent the image extent
     * \param prevData the old swapchain data
     * \return the new swapchain data
     */
    SwapchainData create(ImageExtent extent, std::optional<std::reference_wrapper<SwapchainData const>> prevData);

    /**
     * Vulkan Driver capabilities such as surfaceFormat, presentMode and other related things are cached in an internal
     * structure to allow faster queries.
     * This method allows to refresh this cache by refetching the same properties from the driver again.
     */
    void refreshCapabilities();

    vk::SurfaceFormatKHR const& getSurfaceFormat() const;

private:
    void cacheCapabilities();

    struct SwapchainCache;

    // Must have an exclusive lock on get and set.
    std::unique_ptr<SwapchainCache> m_swapchainCache;
    mutable std::shared_mutex m_cacheMutex;

    PhysicalDevice const& m_physicalDevice;
    LogicalDevice const& m_device;
    Surface const& m_surface;
};

} // namespace ly::renderer
