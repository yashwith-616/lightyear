#include "VulkanFrameContext.h"

#include <bootstrap/Surface.h>
#include <components/FrameContext.h>
#include <components/FrameData.h>
#include <components/SwapchainData.h>
#include <device/LogicalDevice.h>
#include <device/PhysicalDevice.h>
#include <device/QueueFamilyData.h>
#include <ranges>

namespace
{

vk::DependencyInfo transitionImageLayout(
    vk::Image swapchainImage,
    vk::ImageLayout oldLayout,
    vk::ImageLayout newLayout,
    vk::AccessFlags2 srcAccessMask,
    vk::AccessFlags2KHR dstAccessMask,
    vk::PipelineStageFlags2 srcStageMask,
    vk::PipelineStageFlags2 dstStageMask)
{
    vk::ImageMemoryBarrier2 imgBarrier = {
        .srcStageMask = srcStageMask,
        .srcAccessMask = srcAccessMask,
        .dstStageMask = dstStageMask,
        .dstAccessMask = dstAccessMask,
        .oldLayout = oldLayout,
        .newLayout = newLayout,
        .image = swapchainImage,
        .subresourceRange = vk::ImageSubresourceRange{
            .aspectMask = vk::ImageAspectFlagBits::eColor,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1}};

    return {.dependencyFlags = {}, .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &imgBarrier};
}

vk::raii::CommandBuffer&
    getPrimaryCmdBuffer(std::unique_ptr<ly::renderer::CommandRegistry> const& registry, uint32_t qfIndex)
{
    auto cmdExpect = registry->getPrimaryCmdData(qfIndex);
    assert(cmdExpect.has_value() && "No primary command data found");
    auto& primaryBuffer = cmdExpect.value().get().commandBuffers[0];
    return primaryBuffer;
}

} // namespace

namespace ly::renderer
{
using limitUint64 = std::numeric_limits<uint64_t>;

VulkanFrameContext::VulkanFrameContext(
    PhysicalDevice const& physicalDevice, LogicalDevice const& device, Surface const& surface) :
    m_physicalDevice(physicalDevice), m_device(device)
{
    m_swapchainSystem = std::make_unique<SwapchainSystem>(m_physicalDevice, m_device, surface);
}

#pragma region Command Contracts

std::unique_ptr<FrameContext> const&
    VulkanFrameContext::createFrameContext(uint32_t concurrency, uint32_t maxFrameFlights, ImageExtent imageExtent)
{
    m_frameContext = std::make_unique<FrameContext>();
    m_frameContext->currentFrameIndex = 0;

    SwapchainData swapchainData = m_swapchainSystem->create(imageExtent, std::nullopt);
    m_frameContext->swapchain = std::move(swapchainData);

    std::vector<FrameData>& frames = m_frameContext->frames;
    frames.reserve(maxFrameFlights);

    std::ranges::for_each(std::views::iota(0u, maxFrameFlights), [&](uint32_t frameIndex) {
        frames.emplace_back(createPerFrameData(concurrency));
    });

    return m_frameContext;
}

std::unique_ptr<CommandStream> VulkanFrameContext::createQueueSubmissionPipeline(QueueSlot slot)
{
    uint32_t qfIndex = m_device.getQueue(slot).qfIndex;

    std::optional<uint32_t> key;
    for (auto& frame : m_frameContext->frames)
    {
        auto expect = frame.commandRegistry->createCommandPool(qfIndex, CommandType::Primary);
        assert(expect.has_value() && "Pool creation failed");
        key = key.value_or(*expect);
        assert(key == *expect && "key mismatch between registers");
    }

    assert(key.has_value() && "command stream key is not set");

    auto cmdContract = std::make_unique<CommandStream>();
    cmdContract->qfIndex = qfIndex;
    cmdContract->type = CommandType::Primary;
    cmdContract->workerId = *key;

    return std::move(cmdContract);
}

std::unique_ptr<CommandStream>
    VulkanFrameContext::createWorkerSubmissionPipeline(std::unique_ptr<CommandStream>& primaryStream)
{
    std::optional<uint32_t> key;
    for (auto& frame : m_frameContext->frames)
    {
        auto expect = frame.commandRegistry->createCommandPool(primaryStream->qfIndex, CommandType::Secondary);
        assert(expect.has_value() && "worker pool creation failed");
        key = key.value_or(*expect);
        assert(key == *expect && "key mismatch between registers");
    }

    assert(key.has_value() && "command stream key is not set");

    auto cmdContract = std::make_unique<CommandStream>();
    cmdContract->qfIndex = cmdContract->qfIndex;
    cmdContract->type = CommandType::Secondary;
    cmdContract->workerId = *key;

    return std::move(cmdContract);
}

uint32_t VulkanFrameContext::allocateSubmissionRegion(std::unique_ptr<CommandStream>& cmdStream)
{
    std::optional<uint32_t> key;
    for (auto& frame : m_frameContext->frames)
    {
        auto expect = frame.commandRegistry->allocateCommandBuffer(cmdStream);
        assert(expect.has_value() && "Pool creation failed");
        key = key.value_or(*expect);
        assert(key == *expect && "key mismatch between registers");
    }

    assert(key.has_value() && "command buffer not created for cmdStream");
    return *key;
}

#pragma endregion

#pragma region Frame logic

void VulkanFrameContext::beginFrame()
{
    uint32_t qfIndex = m_device.getQueue(QueueSlot::Graphic).qfIndex;
    auto const& registry = getCurrentFrame().commandRegistry;
    auto& primaryBuffer = getPrimaryCmdBuffer(registry, qfIndex);

    // Wait for graphicsQueue
    auto fenceExpect = m_device.getHandle().waitForFences(*getCurrentFrame().drawFence, vk::True, limitUint64::max());
    assert(fenceExpect == vk::Result::eSuccess && "fence captured successfully");
    m_device.getHandle().resetFences(*getCurrentFrame().drawFence);

    // Acquire next semaphore
    auto swapchainExpect = m_frameContext->swapchain.swapchain.acquireNextImage(
        limitUint64::max(), getCurrentFrame().presentCompleteSemaphore, nullptr);
    m_currSwapChainImageIndex = swapchainExpect.second;
    auto const& currImage = m_frameContext->swapchain.images[m_currSwapChainImageIndex];
    auto const& currImageView = m_frameContext->swapchain.imageViews[m_currSwapChainImageIndex];

    // Reset pools
    {
        for (auto& primaryCmd : registry->primaryCmdData())
        {
            primaryCmd.commandPool.reset();
        }

        for (auto& workerCmd : registry->workerCmdData())
        {
            workerCmd.commandPool.reset();
        }
    }

    // command buffer begin()
    {
        // primary buffer begin
        primaryBuffer.begin({.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

        // secondary buffer begin
        vk::CommandBufferInheritanceRenderingInfo renderingInheritance{
            .viewMask = 0,
            .colorAttachmentCount = 1,
            .pColorAttachmentFormats = &m_frameContext->swapchain.colorFormat,
            .depthAttachmentFormat = {},
            .stencilAttachmentFormat = {},
            .rasterizationSamples = vk::SampleCountFlagBits::e1};
        vk::CommandBufferInheritanceInfo inheritance{.pNext = &renderingInheritance};

        for (auto& secondaryCmdBuffer : registry->secondaryCmdBuffers(qfIndex))
        {
            secondaryCmdBuffer.begin(
                {.flags = vk::CommandBufferUsageFlagBits::eRenderPassContinue |
                     vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
                 .pInheritanceInfo = &inheritance});
        }
    }

    // Graphic queue command buffer transition
    {
        // transition from undefined -> colorAttachmentOptimal
        vk::DependencyInfo dependencyInfo = transitionImageLayout(
            currImage,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eColorAttachmentOptimal,
            {},
            vk::AccessFlagBits2::eColorAttachmentWrite,
            vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            vk::PipelineStageFlagBits2::eColorAttachmentOutput);
        primaryBuffer.pipelineBarrier2(dependencyInfo);

        vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);

        vk::RenderingAttachmentInfoKHR attachmentInfoKhr{
            .imageView = currImageView,
            .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .loadOp = vk::AttachmentLoadOp::eClear,
            .storeOp = vk::AttachmentStoreOp::eStore,
            .clearValue = clearColor};

        vk::RenderingInfo renderingInfo{
            .flags = vk::RenderingFlagBits::eContentsSecondaryCommandBuffers,
            .renderArea = {.offset = {0, 0}, .extent = m_frameContext->swapchain.extent},
            .layerCount = 1,
            .viewMask = 0,
            .colorAttachmentCount = 1,
            .pColorAttachments = &attachmentInfoKhr,
            .pDepthAttachment = nullptr,
            .pStencilAttachment = nullptr};

        primaryBuffer.beginRendering(renderingInfo);
    }
}

vk::CommandBuffer const&
    VulkanFrameContext::recordWork(std::unique_ptr<CommandStream> const& cmdStream, std::optional<uint32_t> workerId)
{
    auto cmdDataExpect = getCurrentFrame().commandRegistry->getCommandData(cmdStream);
    assert(cmdDataExpect.has_value() && "Command data query is invalid");
    auto cmdData = cmdDataExpect.value();

    return cmdData->commandBuffers[workerId.value_or(0)];
}

void VulkanFrameContext::endFrame()
{
    uint32_t qfIndex = m_device.getQueue(QueueSlot::Graphic).qfIndex;
    auto const& registry = getCurrentFrame().commandRegistry;
    auto& primaryBuffer = getPrimaryCmdBuffer(registry, qfIndex);

    std::ranges::for_each(registry->secondaryCmdBuffers(qfIndex), [](auto& buffer) { buffer.end(); });

    // EXECUTE SECONDARY BUFFERS
    // May need caching at registry to save performance
    auto secondaryBuffers = registry->secondaryCmdBuffers() | std::ranges::to<std::vector<vk::CommandBuffer>>();
    primaryBuffer.executeCommands(secondaryBuffers);
    primaryBuffer.endRendering();

    // pass the swapchain image to present layer
    vk::Image const& currentImage = m_frameContext->swapchain.images[m_currSwapChainImageIndex];

    // transition from colorAttachment -> presentSrc
    vk::DependencyInfo presentDependency = transitionImageLayout(
        currentImage,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::ImageLayout::ePresentSrcKHR,
        vk::AccessFlagBits2::eColorAttachmentWrite,
        vk::AccessFlagBits2::eNone,
        vk::PipelineStageFlagBits2::eColorAttachmentOutput,
        vk::PipelineStageFlagBits2::eBottomOfPipe);
    primaryBuffer.pipelineBarrier2(presentDependency);
    primaryBuffer.end();
}

void VulkanFrameContext::submit()
{
    auto const& frameData = getCurrentFrame();

    // Submit task to GPU
    auto& primaryBuffer =
        getPrimaryCmdBuffer(getCurrentFrame().commandRegistry, m_device.getQueue(QueueSlot::Graphic).qfIndex);

    // submit graphics queue
    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo submitInfo = {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &*frameData.presentCompleteSemaphore,
        .pWaitDstStageMask = &waitDestinationStageMask,
        .commandBufferCount = 1,
        .pCommandBuffers = &*primaryBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &*frameData.renderCompleteSemaphore,
    };
    m_device.getQueue(QueueSlot::Graphic).handle.submit(submitInfo, *frameData.drawFence);

    // Submit to present queue
    vk::PresentInfoKHR presentInfo = {
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &*frameData.renderCompleteSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &*m_frameContext->swapchain.swapchain,
        .pImageIndices = &m_currSwapChainImageIndex,
    };
    auto presentRes = m_device.getQueue(QueueSlot::Graphic).handle.presentKHR(presentInfo);
    assert(presentRes == vk::Result::eSuccess && "Unexpected result from present call");
    m_frameContext->currentFrameIndex++;
}

#pragma endregion

FrameData const& VulkanFrameContext::getCurrentFrame()
{
    uint32_t frameIndex = m_frameContext->currentFrameIndex % m_maxFlightFrames;
    return m_frameContext->frames[frameIndex];
}

FrameData VulkanFrameContext::createPerFrameData(uint32_t concurrency)
{
    FrameData frameData;
    frameData.commandRegistry = std::make_unique<CommandRegistry>(m_physicalDevice, m_device);

    auto semaphoreExpect = m_device.getHandle().createSemaphore(vk::SemaphoreCreateInfo());
    assert(semaphoreExpect.has_value() && "semaphore couldn't be created");
    frameData.renderCompleteSemaphore = std::move(semaphoreExpect.value());

    semaphoreExpect = m_device.getHandle().createSemaphore(vk::SemaphoreCreateInfo());
    assert(semaphoreExpect.has_value() && "semaphore couldn't be created");
    frameData.presentCompleteSemaphore = std::move(semaphoreExpect.value());

    auto fenceExpect = m_device.getHandle().createFence({.flags = vk::FenceCreateFlagBits::eSignaled});
    assert(fenceExpect.has_value() && "fence couldn't be created");
    frameData.drawFence = std::move(fenceExpect.value());
    return frameData;
}

} // namespace ly::renderer
