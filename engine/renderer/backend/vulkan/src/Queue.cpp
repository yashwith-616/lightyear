#include "Queue.h"

namespace ly::renderer
{


Queue::Queue(PhysicalDevice const& physicalDevice, vk::raii::SurfaceKHR surfaceKhr, QueueType type) {}

vk::raii::Queue Queue::getQueue() {}

uint32_t Queue::acquireGraphicsQueue(PhysicalDevice const& physicalDevice) const
{
    auto queueFamilyProps = physicalDevice.getHandle().getQueueFamilyProperties2();
    auto it = std::ranges::find_if(queueFamilyProps, [](auto& property) {
        auto const& props = property.queueFamilyProperties;
        bool hasGraphicsFlag = (props.queueFlags & vk::QueueFlags{vk::QueueFlagBits::eGraphics}) != vk::QueueFlags{};
        return (props.queueCount > 0 && hasGraphicsFlag);
    });

    // TODO: Update to better assertions
    assert(it != queueFamilyProps.end() && "Failed to find a graphics queue");
    return static_cast<uint32_t>(std::distance(queueFamilyProps.begin(), it));
}

uint32_t Queue::acquirePresentQueue(PhysicalDevice const& physicalDevice, vk::SurfaceKHR surfaceKhr) const
{
    auto const& queueFamilies = physicalDevice.getHandle().getQueueFamilyProperties2();
    for (uint32_t index = 0; index < queueFamilies.size(); ++index)
    {
        auto const& props = queueFamilies[index].queueFamilyProperties;
        bool hasGraphicFlag = props.queueFlags.contains(vk::QueueFlagBits::eGraphics);
        bool hasSurfaceKhr = physicalDevice.getHandle().getSurfaceSupportKHR(index, surfaceKhr);
        if (props.queueCount > 0 && hasGraphicFlag && hasSurfaceKhr)
        {
            return index;
        }
    }

    assert(false && "Present queue not found!");
}

uint32_t Queue::acquireTransferQueue(PhysicalDevice const& physicalDevice) const {}

uint32_t Queue::acquireComputeQueue(PhysicalDevice const& physicalDevice) const {}

} // namespace ly::renderer
