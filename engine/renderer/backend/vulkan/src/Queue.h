#pragma once

#include "PhysicalDevice.h"

namespace ly::renderer
{

/// These are the 4 major types of queue for which the graphic card will be queried for
enum class QueueType
{
    Graphic,
    Present,
    Compute,
    Transfer
};


class Queue
{
public:
    Queue(PhysicalDevice const& physicalDevice, vk::SurfaceKHR surfaceKhr, QueueType type);

    virtual ~Queue() = default;

    QueueType getQueueType() const { return m_type; }

private:
    vk::raii::Queue getQueue();

    uint32_t acquireGraphicsQueue(PhysicalDevice const& physicalDevice) const;
    uint32_t acquirePresentQueue(PhysicalDevice const& physicalDevice, vk::SurfaceKHR surfaceKhr) const;
    uint32_t acquireTransferQueue(PhysicalDevice const& physicalDevice) const;
    uint32_t acquireComputeQueue(PhysicalDevice const& physicalDevice) const;

    vk::raii::Queue m_queue;
    QueueType m_type;
};

} // namespace ly::renderer
