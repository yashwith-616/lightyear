#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "CommandData.h"
#include "SwapchainData.h"
#include "command/CommandRegistry.h"

namespace ly::renderer
{

struct FrameData
{
    vk::raii::Semaphore renderCompleteSemaphore{nullptr};
    vk::raii::Semaphore presentCompleteSemaphore{nullptr};
    vk::raii::Fence drawFence{nullptr};

    std::unique_ptr<CommandRegistry> commandRegistry;
};

} // namespace ly::renderer
