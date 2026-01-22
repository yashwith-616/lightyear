#pragma once
#include <optional>
#include <set>


#include "PhysicalDevice.h"


namespace ly::renderer
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    std::optional<uint32_t> transfer;
    std::optional<uint32_t> compute;

    std::set<uint32_t> getUniqueQueueIndexes() { return {*graphics, *present, *transfer, *compute}; }
};

class LogicalDevice
{
public:
    LogicalDevice(PhysicalDevice const& physicalDevice, vk::SurfaceKHR surface);

    vk::raii::Device const& getHandle() const { return m_device; }

    vk::raii::Queue getGraphicQueue() const { return m_graphicsQueue; }

    vk::raii::Queue getPresentQueue() const { return m_presentQueue; }

    vk::raii::Queue getComputeQueue() const { return m_computeQueue; }

    vk::raii::Queue getTransferQueue() const { return m_transferQueue; }

private:
    vk::raii::Device createLogicalDevice(vk::SurfaceKHR surface);

    void resolveQueueFamilies(vk::SurfaceKHR surface);

    std::vector<vk::DeviceQueueCreateInfo> prepareQueueCreateInfos();

private:
    PhysicalDevice const& m_physicalDevice;

    vk::raii::Device m_device{nullptr};
    QueueFamilyIndices m_queueFamilyIndices{};
    float m_queuePriority{1.f};

    // All queues
    vk::raii::Queue m_graphicsQueue{nullptr};
    vk::raii::Queue m_computeQueue{nullptr};
    vk::raii::Queue m_transferQueue{nullptr};
    vk::raii::Queue m_presentQueue{nullptr};
};

} // namespace ly::renderer
