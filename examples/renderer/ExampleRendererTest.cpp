#include <bootstrap/Instance.h>
#include <device/LogicalDevice.h>
#include <device/PhysicalDevice.h>
#include <systems/VulkanFrameContext.h>

#include "LogScope.hpp"
#include "Window.hpp"
#include "bootstrap/Surface.h"
#include "components/CommandData.h"
#include "device/QueueFamilyData.h"
#include "pipeline/Pipeline.h"
#include "pipeline/Shader.h"

void vulkanRendererExample(Window& window);

int main()
{
    LogScope log;
    Window window;

    // call example function
    vulkanRendererExample(window);

    LOG(logger::Info, "Completed Vulkan renderer example");
}

/// This is the main example showcasing vulkan rendering capabilities
///
/// \param window the glfw window
void vulkanRendererExample(Window& window)
{
    namespace renderer = ly::renderer;

    // Configure all vulkan extensions
    std::vector<std::string> extensions = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName};
    auto winExtensions = window.getVulkanExtensions();

    renderer::InstanceCreateInfo instanceCreateInfo{
        .vulkanValidationLayers = {"VK_LAYER_KHRONOS_validation"},
        .vulkanExtensions = winExtensions,
        .deviceExtensions = extensions,
        .appName = "ExampleRendererTest",
        .appVersion = renderer::Version(1, 0, 0),
        .engineName = "No engine",
        .engineVersion = renderer::Version(1, 0, 0),
    };

    auto vkInstance = renderer::Instance(instanceCreateInfo);
    VkSurfaceKHR windowSurface = window.createWindowSurface(vkInstance);
    auto surface = renderer::Surface(windowSurface);

    auto vkPhysicalDevice = renderer::PhysicalDevice(vkInstance, surface);
    auto vkDevice = renderer::LogicalDevice(vkPhysicalDevice, surface);

    // Initialize vulkan context
    auto vkFrameContext = renderer::VulkanFrameContext(vkPhysicalDevice, vkDevice, surface);
    renderer::ImageExtent imgExtent{.height = 720, .width = 1280};
    vkFrameContext.init(1, imgExtent);
    auto mainVulkanCmd = vkFrameContext.createQueueSubmissionPipeline(renderer::QueueSlot::Graphic);
    auto mainSubmissionKey = vkFrameContext.allocateSubmissionRegion(mainVulkanCmd);

    // Pipeline creation
    std::filesystem::path shaderPath{"F:/workspace/cpp/lightyear/examples/renderer/assets/Shader.spv"};
    renderer::Shader shader(vkDevice, shaderPath);

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = {
        shader.createSubShader(vk::ShaderStageFlagBits::eVertex, "vertMain"),
        shader.createSubShader(vk::ShaderStageFlagBits::eFragment, "fragMain")};

    auto graphicPipeline = renderer::Pipeline(vkDevice);
    graphicPipeline.createPipeline(
        shaderStages, vk::Extent2D(imgExtent.width, imgExtent.height), vkFrameContext.getSurfaceFormat());

    // Till window close loop through this
    while (!window.shouldWindowClose())
    {
        window.pollEvents();
        vkFrameContext.beginFrame();

        auto mainCmd = vkFrameContext.recordWork(mainVulkanCmd, mainSubmissionKey);
        mainCmd.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicPipeline.getPipeline());
        mainCmd.setViewport(
            0,
            vk::Viewport(
                0.0f, 0.0f, static_cast<float>(imgExtent.width), static_cast<float>(imgExtent.height), 0.0f, 1.0f));
        mainCmd.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(imgExtent.width, imgExtent.height)));
        mainCmd.draw(3, 1, 0, 0);

        vkFrameContext.endFrame();
        vkFrameContext.submit();
    }
}
