#pragma once

#include "FrameData.h"
#include "SwapchainData.h"

namespace ly::renderer
{

struct FrameContext
{
    SwapchainData swapchain;
    std::vector<FrameData> frames{};

    uint32_t currentFrameIndex{};
};

} // namespace ly::renderer
