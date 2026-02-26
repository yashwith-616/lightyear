#include "CommandRegistry.h"

#include <vulkan/vulkan_raii.hpp>
#include "device/LogicalDevice.h"
#include "device/PhysicalDevice.h"

namespace
{

vk::CommandBufferLevel toVkLevel(ly::renderer::CommandType type)
{
    using ct = ly::renderer::CommandType;
    switch (type)
    {
    case ct::Primary:
        return vk::CommandBufferLevel::ePrimary;
    case ct::Secondary:
        return vk::CommandBufferLevel::eSecondary;
    }

    std::unreachable();
}

vk::raii::CommandPool createCommandPool(vk::raii::Device const& device, uint32_t qfIndex)
{
    vk::CommandPoolCreateInfo cmdPoolCreateInfo{
        .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer, .queueFamilyIndex = qfIndex};

    auto expect = device.createCommandPool(cmdPoolCreateInfo);
    assert(expect.has_value() && "Pool creation failed");
    return std::move(expect.value());
}

std::vector<vk::raii::CommandBuffer> allocateCommandBuffers(
    vk::raii::Device const& device, vk::raii::CommandPool const& pool, ly::renderer::CommandType type)
{
    vk::CommandBufferAllocateInfo allocateInfo{.commandPool = *pool, .level = toVkLevel(type), .commandBufferCount = 1};
    auto expect = device.allocateCommandBuffers(allocateInfo);
    assert(expect.has_value() && "Command buffer allocation failed");
    return std::move(expect.value());
}

} // namespace

namespace ly::renderer
{

CommandRegistry::CommandRegistry(PhysicalDevice const& physicalDevice, LogicalDevice const& device) :
    m_physicalDevice(physicalDevice), m_device(device)
{}

CommandRegistry::~CommandRegistry() = default;

std::expected<uint32_t, RegistryError> CommandRegistry::createCommandPool(uint32_t qfIndex, CommandType type)
{
    auto queuePoolIt = m_registry.find(qfIndex);
    auto queuePoolOpt = (queuePoolIt != m_registry.end()) ? std::optional{std::ref(queuePoolIt->second)} : std::nullopt;

    switch (type)
    {
    case CommandType::Primary:
        {
            assert(!queuePoolOpt.has_value() && "Primary command data can be initialized only once");
            CommandData cmdData{
                .qfIndex = qfIndex,
                .type = type,
                .commandPool = std::move(::createCommandPool(m_device.getHandle(), qfIndex))};
            QueueCommandPool cmdPool{.qfIndex = qfIndex, .primary = std::move(cmdData)};
            m_registry[qfIndex] = std::move(cmdPool);
            return 0;
        }
    case CommandType::Secondary:
        {
            assert(queuePoolOpt.has_value() && "Primary command data must be in initialized before creating secondary");
            auto& queueCmdPool = (*queuePoolOpt).get();

            CommandData workerCmdData{
                .qfIndex = qfIndex,
                .type = type,
                .commandPool = std::move(::createCommandPool(m_device.getHandle(), qfIndex))};
            queueCmdPool.workers.push_back(std::move(workerCmdData));
            return queueCmdPool.workerCount++;
        }
    }

    std::unreachable();
}

std::expected<uint32_t, RegistryError>
    CommandRegistry::allocateCommandBuffer(std::unique_ptr<CommandStream> const& stream)
{
    auto queuePoolIt = m_registry.find(stream->qfIndex);
    if (queuePoolIt == m_registry.end())
    {
        return std::unexpected(RegistryError::NotFound);
    }

    switch (stream->type)
    {
    case CommandType::Primary:
        {
            auto& primaryCmd = queuePoolIt->second.primary;
            assert(primaryCmd.commandPool != nullptr && "Command pool is not yet allocated");
            assert(primaryCmd.commandBuffers.empty() && "Cannot allocate more than one primary command buffer");
            auto newBuffers =
                ::allocateCommandBuffers(m_device.getHandle(), primaryCmd.commandPool, CommandType::Primary);
            assert(newBuffers.size() > 1 && "Multiple buffers cannot be allocated at the same time");
            std::ranges::move(newBuffers, std::back_inserter(primaryCmd.commandBuffers));
            return primaryCmd.commandBuffers.size() - 1;
        }
    case CommandType::Secondary:
        {
            auto& queuePool = queuePoolIt->second;
            assert(queuePool.workerCount >= stream->workerId && "WorkerId exceeds the worker counts");
            auto& workerCmd = queuePool.workers[stream->workerId];

            assert(workerCmd.commandPool != nullptr && "Command pool is not yet allocated");
            auto newBuffers =
                ::allocateCommandBuffers(m_device.getHandle(), workerCmd.commandPool, CommandType::Secondary);
            assert(newBuffers.size() > 1 && "Multiple buffers cannot be allocated at the same time");
            std::ranges::move(newBuffers, std::back_inserter(workerCmd.commandBuffers));
            return workerCmd.commandBuffers.size() - 1;
        }
    }

    std::unreachable();
}

std::expected<CommandData const*, RegistryError>
    CommandRegistry::getCommandData(std::unique_ptr<CommandStream> const& stream) const
{
    auto queuePoolIt = m_registry.find(stream->qfIndex);
    if (queuePoolIt == m_registry.end())
    {
        return std::unexpected(RegistryError::NotFound);
    }

    if (stream->type == CommandType::Primary)
    {
        return &queuePoolIt->second.primary;
    }

    if (stream->type == CommandType::Secondary)
    {
        auto& queuePool = queuePoolIt->second;
        assert(queuePool.workerCount >= stream->workerId && "WorkerId exceeds the worker counts");
        return &queuePool.workers[stream->workerId];
    }

    std::unreachable();
}

CommandRegistry CommandRegistry::clone()
{
    registry newRegistry;
    newRegistry.reserve(m_registry.size());

    // TODO: Need to complete this
    return CommandRegistry(m_physicalDevice, m_device, std::move(newRegistry));
}

void CommandRegistry::clear() { m_registry.clear(); }

CommandRegistry::CommandRegistry(PhysicalDevice const& physicalDevice, LogicalDevice const& device, registry registry) :
    m_physicalDevice(physicalDevice), m_device(device), m_registry(std::move(registry))
{}

} // namespace ly::renderer
