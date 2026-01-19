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
    PhysicalDevice(vk::raii::Instance const& vulkanInstance);

    vk::raii::PhysicalDevice const& getHandle() const { return m_device; };

    std::vector<vk::QueueFamilyProperties2> getQueueFamilyProperties() const;

    FeatureStorage getRequiredDeviceFeatures() const;

    std::vector<char const*> getRequiredDeviceExtensions() const;

private:
    vk::raii::PhysicalDevice pickPhysicalDevice(vk::raii::Instance const& vulkanInstance);

    uint32_t getDeviceTypeScore(vk::PhysicalDeviceType deviceType);

private:
    vk::raii::PhysicalDevice m_device;
};

} // namespace ly::renderer
