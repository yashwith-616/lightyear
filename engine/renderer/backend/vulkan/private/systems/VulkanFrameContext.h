#pragma once
#include <complex.h>

#include <components/FrameContext.h>
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

    void init(uint32_t maxFrameFlights, ImageExtent imageExtent);

    std::unique_ptr<CommandStream> createQueueSubmissionPipeline(QueueSlot slot);

    std::unique_ptr<CommandStream> createWorkerSubmissionPipeline(std::unique_ptr<CommandStream>& primaryStream);

    uint32_t allocateSubmissionRegion(std::unique_ptr<CommandStream>& cmdStream);

    void beginFrame();

    vk::CommandBuffer recordWork(std::unique_ptr<CommandStream> const& cmdStream, std::optional<uint32_t> workerId);

    void endFrame();

    void submit();

    vk::SurfaceFormatKHR const& getSurfaceFormat() const;

private:
    std::unique_ptr<FrameData> createPerFrameData();

    std::unique_ptr<FrameData> const& getCurrentFrame();

    PhysicalDevice const& m_physicalDevice;
    LogicalDevice const& m_device;

    std::unique_ptr<SwapchainSystem> m_swapchainSystem;
    std::unique_ptr<FrameContext> m_frameContext;
    uint32_t m_maxFlightFrames{1};

    uint32_t m_currSwapChainImageIndex{0};
};

} // namespace ly::renderer
