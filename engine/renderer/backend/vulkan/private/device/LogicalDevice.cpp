#pragma once

#include "LogicalDevice.h"
#include <cassert>

namespace ly::renderer
{

LogicalDevice::LogicalDevice(PhysicalDevice const& physicalDevice, std::optional<vk::SurfaceKHR> surface) :
    m_device(createLogicalDevice(physicalDevice, std::move(surface)))
{
    m_graphicsQueue = vk::raii::Queue(m_device, m_queueFamilyIndices.graphics.value(), 0);
    m_computeQueue = vk::raii::Queue(m_device, m_queueFamilyIndices.compute.value(), 0);
    m_transferQueue = vk::raii::Queue(m_device, m_queueFamilyIndices.transfer.value(), 0);
    m_presentQueue = vk::raii::Queue(m_device, m_queueFamilyIndices.present.value(), 0);
}


vk::raii::Device
    LogicalDevice::createLogicalDevice(PhysicalDevice const& physicalDevice, std::optional<vk::SurfaceKHR> surface)
{
    assert(surface.has_value() && "Surface has not been provided");
    resolveQueueFamilies(physicalDevice, std::move(surface.value()));

    FeatureStorage deviceFeatures = physicalDevice.getRequiredDeviceFeatures();
    auto const& deviceExtensions = physicalDevice.getRequiredDeviceExtensions();
    auto queueInfos = prepareQueueCreateInfos();

    vk::DeviceCreateInfo deviceCreateInfo{
        .pNext = &deviceFeatures.core,
        .queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size()),
        .pQueueCreateInfos = queueInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data()};
    return vk::raii::Device(physicalDevice.getHandle(), deviceCreateInfo);
}

/// Search for all queue families and resolve the indexes for each queue
///
/// \param physicalDevice physical device
/// \param surface the surface
void LogicalDevice::resolveQueueFamilies(PhysicalDevice const& physicalDevice, vk::SurfaceKHR surface)
{
    auto const& queueProps = physicalDevice.getQueueFamilyProperties();
    auto hasSurfaceSupport = [&](uint32_t queueFamilyIndex) {
        return physicalDevice.getHandle().getSurfaceSupportKHR(queueFamilyIndex, std::move(surface));
    };

    auto hasGraphicSupport = [&](uint32_t queueFamilyIndex) {
        vk::QueueFlags qfIndexFlags = queueProps[queueFamilyIndex].queueFamilyProperties.queueFlags;
        return static_cast<bool>(qfIndexFlags & vk::QueueFlagBits::eGraphics);
    };

    auto hasComputeSupport = [&](uint32_t queueFamilyIndex) {
        vk::QueueFlags qfIndexFlags = queueProps[queueFamilyIndex].queueFamilyProperties.queueFlags;
        return static_cast<bool>(qfIndexFlags & vk::QueueFlagBits::eCompute);
    };

    auto hasTransferSupport = [&](uint32_t queueFamilyIndex) {
        vk::QueueFlags qfIndexFlags = queueProps[queueFamilyIndex].queueFamilyProperties.queueFlags;
        return static_cast<bool>(qfIndexFlags & vk::QueueFlagBits::eTransfer);
    };

    uint32_t qFamilySize = queueProps.size();
    for (uint32_t index{0}; index < qFamilySize; index++)
    {
        bool presentFound = hasSurfaceSupport(index);
        bool graphicFound = hasGraphicSupport(index);
        bool computeFound = hasComputeSupport(index);
        bool transferFound = hasTransferSupport(index);

        if (presentFound && graphicFound)
        {
            m_queueFamilyIndices.present = index;
            m_queueFamilyIndices.graphics = index;
        }
        else if (graphicFound && !m_queueFamilyIndices.graphics.has_value())
        {
            m_queueFamilyIndices.graphics = index;
        }
        else if (presentFound && !m_queueFamilyIndices.present.has_value())
        {
            m_queueFamilyIndices.present = index;
        }

        if (computeFound)
        {
            if (!m_queueFamilyIndices.compute.has_value() || !graphicFound)
            {
                m_queueFamilyIndices.compute = index;
            }
        }

        if (transferFound)
        {
            if (!m_queueFamilyIndices.transfer.has_value() || (!graphicFound && !computeFound))
            {
                m_queueFamilyIndices.transfer = index;
            }
        }
    }
}

std::vector<vk::DeviceQueueCreateInfo> LogicalDevice::prepareQueueCreateInfos()
{
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    auto uniqueIndices = m_queueFamilyIndices.getUniqueQueueIndexes();

    for (uint32_t index : uniqueIndices)
    {
        // Use the member variable address so it stays alive
        vk::DeviceQueueCreateInfo createInfo{
            .queueFamilyIndex = index, .queueCount = 1, .pQueuePriorities = &m_queuePriority};

        queueCreateInfos.push_back(createInfo);
    }

    assert(queueCreateInfos.size() > 0 && "Device queue create info is empty");
    return queueCreateInfos;
}

} // namespace ly::renderer
