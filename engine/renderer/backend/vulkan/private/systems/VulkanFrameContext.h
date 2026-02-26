#pragma once
#include <complex.h>

#include "SwapchainSystem.h"
#include "command/CommandRegistry.h"

namespace ly::renderer
{

class LogicalDevice;
class PhysicalDevice;
class Surface;
struct FrameData;
struct FrameContext;
enum class QueueSlot : uint8_t;

class VulkanFrameContext
{
public:
    VulkanFrameContext(PhysicalDevice const& physicalDevice, LogicalDevice const& device, Surface const& surface);

    ~VulkanFrameContext() = default;

    std::unique_ptr<FrameContext> const&
        createFrameContext(uint32_t concurrency, uint32_t maxFrameFlights, ImageExtent imageExtent);

    std::unique_ptr<CommandStream> createQueueSubmissionPipeline(QueueSlot slot);

    std::unique_ptr<CommandStream> createWorkerSubmissionPipeline(std::unique_ptr<CommandStream>& primaryStream);

    uint32_t allocateSubmissionRegion(std::unique_ptr<CommandStream>& cmdStream);

    void beginFrame();

    vk::CommandBuffer const&
        recordWork(std::unique_ptr<CommandStream> const& cmdStream, std::optional<uint32_t> workerId);

    void endFrame();

    void submit();

    FrameData const& getCurrentFrame();

private:
    FrameData createPerFrameData(uint32_t concurrency);

    PhysicalDevice const& m_physicalDevice;
    LogicalDevice const& m_device;

    std::unique_ptr<SwapchainSystem> m_swapchainSystem;
    std::unique_ptr<FrameContext> m_frameContext;
    uint32_t m_maxFlightFrames{1};

    uint32_t m_currSwapChainImageIndex{0};
};

} // namespace ly::renderer
