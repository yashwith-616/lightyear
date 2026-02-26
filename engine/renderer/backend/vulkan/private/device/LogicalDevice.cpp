#include "LogicalDevice.h"
#include "QueueFamilyData.h"

#include <cassert>

namespace
{

ly::renderer::QueueTypeFlag
    getQueueFamilyFlags(uint32_t queueIndex, std::vector<vk::QueueFamilyProperties2> const& queueProps)
{
    using ly::renderer::QueueTypeFlag;

    auto vkFlags = queueProps[queueIndex].queueFamilyProperties.queueFlags;
    auto result = QueueTypeFlag::None;

    if (vkFlags & vk::QueueFlagBits::eGraphics)
        result |= QueueTypeFlag::Graphic;
    if (vkFlags & vk::QueueFlagBits::eCompute)
        result |= QueueTypeFlag::Compute;
    if (vkFlags & vk::QueueFlagBits::eTransfer)
        result |= QueueTypeFlag::Transfer;

    return result;
}

auto hasSurfaceSupport =
    [](vk::raii::PhysicalDevice const& physicalDevice, uint32_t queueFamilyIndex, vk::SurfaceKHR const& surface) {
        return physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, surface);
    };

} // namespace

namespace ly::renderer
{

static std::pair<QueueTypeFlag, QueueSlot> const k_mapping[] = {
    {QueueTypeFlag::Graphic, QueueSlot::Graphic},
    {QueueTypeFlag::Present, QueueSlot::Present},
    {QueueTypeFlag::Compute, QueueSlot::Compute},
    {QueueTypeFlag::Transfer, QueueSlot::Transfer}};

LogicalDevice::LogicalDevice(PhysicalDevice const& physicalDevice, Surface const& surface) :
    m_physicalDevice(physicalDevice)
{
    m_device = std::move(createLogicalDevice(surface));

    for (std::unique_ptr<QueueFamilyData> const& data : m_supportedQueues)
    {
        auto queueExpect = m_device.getQueue(data->familyIndex, data->internalIndex);
        assert(queueExpect.has_value() && "Device queue could not be fetched");
        auto queueHandle = std::make_shared<QueueHandle>(std::move(queueExpect.value()), data->internalIndex);

        std::ranges::for_each(k_mapping, [&](auto const& map) {
            auto [flag, slot] = map;
            if (test(data->flags, flag))
            {
                m_allQueues[static_cast<size_t>(slot)] = queueHandle;
            }
        });
    }
}

std::deque<std::unique_ptr<QueueFamilyData>> const& LogicalDevice::getSupportedQueues() const
{
    return m_supportedQueues;
}

QueueHandle const& LogicalDevice::getQueue(QueueSlot slot) const
{
    auto const& queue = m_allQueues[static_cast<size_t>(slot)];
    assert(queue != nullptr && "Requested queue type is not supported by this device");
    return *queue;
}

vk::raii::Device LogicalDevice::createLogicalDevice(Surface const& surface)
{
    resolveQueueFamilies(surface);
    FeatureStorage deviceFeatures = m_physicalDevice.getRequiredDeviceFeatures();
    auto const& deviceExtensions = m_physicalDevice.getRequiredDeviceExtensions();

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::ranges::transform(
        m_supportedQueues, std::back_inserter(queueCreateInfos), [](std::unique_ptr<QueueFamilyData> const& data) {
            vk::DeviceQueueCreateInfo createInfo{
                .queueFamilyIndex = data->familyIndex, .queueCount = 1, .pQueuePriorities = &data->priority};
            return createInfo;
        });
    assert(queueCreateInfos.size() > 0 && "Device queue create info is empty");

    vk::DeviceCreateInfo deviceCreateInfo{
        .pNext = &deviceFeatures.core,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
        .ppEnabledExtensionNames = deviceExtensions.data()};

    auto deviceExpected = m_physicalDevice.getHandle().createDevice(deviceCreateInfo);
    assert(deviceExpected.has_value() && "Device was not successfully created");
    return std::move(deviceExpected.value());
}

/// Search for all queue families and resolve the indexes for each queue
///
/// \param physicalDevice physical device
/// \param surface the surface
void LogicalDevice::resolveQueueFamilies(Surface const& surface)
{
    auto totalFound = QueueTypeFlag::None;
    auto queueProps = m_physicalDevice.getQueueFamilyProperties();

    for (uint32_t index{0}; index < queueProps.size(); index++)
    {
        QueueTypeFlag supported = getQueueFamilyFlags(index, queueProps);
        if (hasSurfaceSupport(m_physicalDevice.getHandle(), index, surface.getHandle()))
        {
            supported |= QueueTypeFlag::Graphic;
        }

        if (supported == QueueTypeFlag::None)
        {
            continue;
        }

        // Populate queue family data
        auto data = std::make_unique<QueueFamilyData>();
        data->familyIndex = index;

        auto claimedByCurrentIndex = QueueTypeFlag::None;

        if (!hasAny(totalFound, QueueTypeFlag::Graphic | QueueTypeFlag::Present))
        {
            if (hasAll(supported, QueueTypeFlag::Graphic | QueueTypeFlag::Present))
            {
                data->addSupport(QueueTypeFlag::Graphic | QueueTypeFlag::Present);
                claimedByCurrentIndex |= (QueueTypeFlag::Graphic | QueueTypeFlag::Present);
            }
        }

        // If any flags in missing but are supported, add them to the data
        QueueTypeFlag missing = QueueTypeFlag::All & ~(totalFound | claimedByCurrentIndex);
        claimedByCurrentIndex |= (supported & missing);

        if (claimedByCurrentIndex != QueueTypeFlag::None)
        {
            data->addSupport(claimedByCurrentIndex);
            totalFound |= claimedByCurrentIndex;

            m_supportedQueues.push_back(std::move(data));
        }

        if (hasAll(totalFound, QueueTypeFlag::All))
        {
            break;
        }
    }
}

} // namespace ly::renderer
