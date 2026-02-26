#include "PhysicalDevice.h"

#include <cassert>
#include <expected>
#include <map>
#include <vector>

namespace ly::renderer
{

PhysicalDevice::PhysicalDevice(Instance const& vulkanInstance, Surface const& surface) :
    m_vulkanInstance(vulkanInstance), m_device(pickPhysicalDevice(vulkanInstance))
{
    m_cacheQueueProperties = m_device.getQueueFamilyProperties2();
    m_cachedSurfaceFormatsKhr = m_device.getSurfaceFormatsKHR(surface.getHandle());
    m_cachedPresentModesKhr = m_device.getSurfacePresentModesKHR(surface.getHandle());
    m_cachedSurfaceCapabilities = m_device.getSurfaceCapabilitiesKHR(surface.getHandle());
}

FeatureStorage PhysicalDevice::getRequiredDeviceFeatures() const
{
    FeatureStorage storage{};
    storage.core.features = {};
    storage.core.features.samplerAnisotropy = true;
    storage.core.features.shaderInt64 = true;
    storage.core.features.multiDrawIndirect = true;

    // Vulkan 1.2 Features (The "Bindless" Engine)
    storage.v12.bufferDeviceAddress = true;
    storage.v12.descriptorIndexing = true;
    storage.v12.runtimeDescriptorArray = true;
    storage.v12.descriptorBindingPartiallyBound = true;
    storage.v12.descriptorBindingVariableDescriptorCount = true;
    storage.v12.shaderSampledImageArrayNonUniformIndexing = true;

    // Vulkan 1.3 Features (The "Quality of Life" Upgrade)
    storage.v13.dynamicRendering = true;
    storage.v13.synchronization2 = true;
    storage.v13.maintenance4 = true;
    storage.link();

    return storage;
}

/// TODO: Required to use weighted average cost where different weight is added to each category to determine the final
/// score. Each category itself gets a normalized score between 0-1
///
/// \param vulkanInstance the vulkan instance
/// \return the selected physical device
vk::raii::PhysicalDevice PhysicalDevice::pickPhysicalDevice(Instance const& vulkanInstance)
{
    auto devices = vulkanInstance.getInstance().enumeratePhysicalDevices();

    // TODO: Need to change the below code to make sure debugbreak and crash is performed.
    assert(devices.has_value() && "Physical device is not available");

    // Move to a better scoring pattern going forward.
    std::multimap<int, vk::raii::PhysicalDevice> candidates;

    for (auto& device : devices.value())
    {
        vk::PhysicalDeviceProperties2 props = device.getProperties2();
        vk::PhysicalDeviceFeatures2 features = device.getFeatures2();
        uint32_t score{0};

        score += getDeviceTypeScore(props.properties.deviceType);
        score += props.properties.limits.maxImageDimension2D;

        candidates.insert(std::make_pair(score, device));
    }

    assert(candidates.rbegin()->first <= 0 && "Physical device candidates did not score abo");
    return std::move(candidates.rbegin()->second);
}

uint32_t PhysicalDevice::getDeviceTypeScore(vk::PhysicalDeviceType deviceType)
{
    switch (deviceType)
    {
    case vk::PhysicalDeviceType::eDiscreteGpu:
        return 1000;
    case vk::PhysicalDeviceType::eIntegratedGpu:
        return 100;
    case vk::PhysicalDeviceType::eVirtualGpu:
        return 20;
    default:
        return 0;
    }
}


} // namespace ly::renderer
