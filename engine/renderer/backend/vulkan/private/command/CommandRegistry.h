#pragma once

#include <ankerl/unordered_dense.h>
#include <expected>
#include <ranges>
#include "components/CommandData.h"

namespace ly::renderer
{

class PhysicalDevice;
class LogicalDevice;

/// Data structure as required by ankerl unordered_dense
struct IdentityHash
{
    // NOLINTNEXTLINE
    using is_avalanching = void;

    [[nodiscard]] uint64_t operator()(uint64_t key) const noexcept { return key; }
};

enum class RegistryError
{
    NotFound,
};

/// Register all command pools and command buffers created
class CommandRegistry
{
    using registry = ankerl::unordered_dense::map<uint32_t, QueueCommandPool>;

public:
    CommandRegistry(PhysicalDevice const& physicalDevice, LogicalDevice const& device);
    virtual ~CommandRegistry();

    CommandRegistry(CommandRegistry const&) = delete;
    CommandRegistry& operator=(CommandRegistry const&) = delete;
    CommandRegistry(CommandRegistry&&) = default;
    CommandRegistry& operator=(CommandRegistry&&) = delete;

    std::expected<uint32_t, RegistryError> createCommandPool(uint32_t qfIndex, CommandType type);
    std::expected<uint32_t, RegistryError> allocateCommandBuffer(std::unique_ptr<CommandStream> const& stream);
    std::expected<CommandData const*, RegistryError> getCommandData(std::unique_ptr<CommandStream> const& streamKey) const;

    /**
     * The command registry entries will be cloned and the existing key will be used for mapping the new vulkan buffers
     * and pools
     *
     * \return New command registry
     */
    CommandRegistry clone();

    void clear();

    auto primaryCmdData()
    {
        // clang-format off
        return m_registry
            | std::views::values
            | std::views::transform([](auto const& queuePool) -> auto& { return queuePool.primary; });
        // clang-format on
    }

    std::expected<std::reference_wrapper<CommandData>, RegistryError> getPrimaryCmdData(uint32_t qfIndex)
    {
        auto it = m_registry.find(qfIndex);
        if (it != m_registry.end())
        {
            return it->second.primary;
        }
        return std::unexpected{RegistryError::NotFound};
    }

    auto workerCmdData()
    {
        // clang-format off
        return m_registry
            | std::views::values
            | std::views::transform([](auto const& queuePool) -> auto& { return queuePool.workers; })
            | std::views::join;
        // clang-format on
    }

    auto primaryCmdBuffers()
    {
        // clang-format off
        return m_registry
            | std::views::values
            | std::views::transform([](auto const& queuePool) -> auto& { return queuePool.primary; })
            | std::views::transform([](auto const& cmdData) -> auto& {return cmdData.commandBuffers})
            | std::views::join;
        // clang-format on
    }

    auto secondaryCmdBuffers()
    {
        // clang-format off
        return m_registry
            | std::views::values
            | std::views::transform([](auto const& queuePool) -> auto& { return queuePool.workers; })
            | std::views::join
            | std::views::transform([](auto const& workers) -> auto& { return workers.commandBuffers; })
            | std::views::join;
        // clang-format on
    }

    auto secondaryCmdBuffers(uint32_t queueIndex)
    {
        // clang-format off
        return m_registry
            | std::views::values
            | std::views::filter([queueIndex](auto const& queuePool) -> bool {
                return queuePool.qfIndex == queueIndex;
            })
            | std::views::transform([](auto const& queuePool) -> auto& {
                return queuePool.workers;
            })
            | std::views::join
            | std::views::transform([](auto const& workers) -> auto& { return workers.commandBuffers; })
            | std::views::join;
        // clang-format on
    }

private:
    CommandRegistry(PhysicalDevice const& physicalDevice, LogicalDevice const& device, registry registry);

private:
    registry m_registry;
    PhysicalDevice const& m_physicalDevice;
    LogicalDevice const& m_device;
};

} // namespace ly::renderer
