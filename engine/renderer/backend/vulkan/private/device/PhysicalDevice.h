#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace ly::renderer
{

struct FeatureStorage
{
    vk::PhysicalDeviceFeatures2 core;
    vk::PhysicalDeviceVulkan12Features v12;
    vk::PhysicalDeviceVulkan13Features v13;

    void link()
    {
        core.pNext = &v12;
        v12.pNext = &v13;
        v13.pNext = nullptr;
    }
};


class PhysicalDevice
{
public:
    PhysicalDevice(vk::raii::Instance const& vulkanInstance, vk::raii::SurfaceKHR surface);

    FeatureStorage getRequiredDeviceFeatures() const;

    std::vector<char const*> getRequiredDeviceExtensions() const;

    vk::raii::PhysicalDevice const& getHandle() const { return m_device; }

    std::vector<vk::QueueFamilyProperties2> const& getQueueFamilyProperties() const { return m_cacheQueueProperties; }

    std::vector<vk::SurfaceFormatKHR> const& getSurfaceFormatsKhr() const { return m_cachedSurfaceFormatsKhr; }

    std::vector<vk::PresentModeKHR> const& getPresentModesKhr() const { return m_cachedPresentModesKhr; }

    vk::SurfaceCapabilities2KHR const& getSurfaceCapabilities() const { return m_cachedSurfaceCapabilities; }

private:
    vk::raii::PhysicalDevice pickPhysicalDevice(vk::raii::Instance const& vulkanInstance);

    uint32_t getDeviceTypeScore(vk::PhysicalDeviceType deviceType);

private:
    vk::raii::PhysicalDevice m_device{nullptr};
    vk::raii::SurfaceKHR m_surface;

    // cached features, to make sure the fetch happens only once
    std::vector<vk::QueueFamilyProperties2> m_cacheQueueProperties{};
    std::vector<vk::SurfaceFormatKHR> m_cachedSurfaceFormatsKhr{};
    std::vector<vk::PresentModeKHR> m_cachedPresentModesKhr{};
    vk::SurfaceCapabilities2KHR m_cachedSurfaceCapabilities{};
};

} // namespace ly::renderer
