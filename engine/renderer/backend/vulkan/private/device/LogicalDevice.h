#pragma once
#include <array>
#include <deque>
#include <optional>
#include <set>

#include "PhysicalDevice.h"


namespace ly::renderer
{

struct QueueFamilyData;
enum class QueueTypeFlag : uint8_t;
enum class QueueSlot : uint8_t;

struct QueueHandle
{
    vk::raii::Queue handle;
    uint32_t qfIndex;
};

class LogicalDevice
{
public:
    LogicalDevice(PhysicalDevice const& physicalDevice, Surface const& surface);

    vk::raii::Device const& getHandle() const { return m_device; }

    std::deque<std::unique_ptr<QueueFamilyData>> const& getSupportedQueues() const;

    [[nodiscard]] QueueHandle const& getQueue(QueueSlot slot) const;

private:
    vk::raii::Device createLogicalDevice(Surface const& surface);

    void resolveQueueFamilies(Surface const& surface);

private:
    PhysicalDevice const& m_physicalDevice;

    vk::raii::Device m_device{nullptr};

    // Switch to unique pointers
    std::deque<std::unique_ptr<QueueFamilyData>> m_supportedQueues;

    // All queues
    std::array<std::shared_ptr<QueueHandle>, 4> m_allQueues;
};

} // namespace ly::renderer
