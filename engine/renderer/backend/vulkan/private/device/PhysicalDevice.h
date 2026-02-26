#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "bootstrap/Instance.h"
#include "bootstrap/Surface.h"

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
    PhysicalDevice(Instance const& vulkanInstance, Surface const& surface);

    FeatureStorage getRequiredDeviceFeatures() const;

    std::vector<char const*> getRequiredDeviceExtensions() const { return m_vulkanInstance.getVulkanExtensions(); }

    vk::raii::PhysicalDevice const& getHandle() const { return m_device; }

    std::vector<vk::QueueFamilyProperties2> const& getQueueFamilyProperties() const { return m_cacheQueueProperties; }

    std::vector<vk::SurfaceFormatKHR> const& getSurfaceFormatsKhr() const { return m_cachedSurfaceFormatsKhr; }

    std::vector<vk::PresentModeKHR> const& getPresentModesKhr() const { return m_cachedPresentModesKhr; }

    vk::SurfaceCapabilitiesKHR const& getSurfaceCapabilities() const { return m_cachedSurfaceCapabilities; }

private:
    vk::raii::PhysicalDevice pickPhysicalDevice(Instance const& vulkanInstance);

    uint32_t getDeviceTypeScore(vk::PhysicalDeviceType deviceType);

private:
    Instance const& m_vulkanInstance;
    vk::raii::PhysicalDevice m_device{nullptr};

    // cached features, to make sure the fetch happens only once
    std::vector<vk::QueueFamilyProperties2> m_cacheQueueProperties{};
    std::vector<vk::SurfaceFormatKHR> m_cachedSurfaceFormatsKhr{};
    std::vector<vk::PresentModeKHR> m_cachedPresentModesKhr{};
    vk::SurfaceCapabilitiesKHR m_cachedSurfaceCapabilities{};
};

} // namespace ly::renderer
