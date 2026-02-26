#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "command/CommandType.h"

namespace ly::renderer
{

struct CommandData
{
    uint32_t qfIndex{};
    CommandType type{CommandType::Primary};
    vk::raii::CommandPool commandPool{nullptr};
    std::vector<vk::raii::CommandBuffer> commandBuffers{};

    // Add a restriction on number of buffers to be created
};

struct QueueCommandPool
{
    uint32_t qfIndex;
    CommandData primary;
    std::vector<CommandData> workers;
    uint32_t workerCount{0};
};

struct CommandStream
{
    uint32_t qfIndex{0};
    CommandType type{CommandType::Primary};
    uint32_t workerId{0};

    CommandStream();
    virtual ~CommandStream() = default;
    
    CommandStream(CommandStream const&) = delete;
    CommandStream& operator=(CommandStream const&) = delete;

    CommandStream(CommandStream&&) = default;
    CommandStream& operator=(CommandStream&&) = default;
};

} // namespace ly::renderer
