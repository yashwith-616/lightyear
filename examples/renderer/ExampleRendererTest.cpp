#include "LogScope.hpp"
#include "Window.hpp"

#include <bootstrap/Instance.h>
#include <device/LogicalDevice.h>
#include <device/PhysicalDevice.h>

#include "bootstrap/Surface.h"
#include "swapchain/Swapchain.h"

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
    std::ranges::copy(winExtensions, std::back_inserter(extensions));

    renderer::InstanceCreateInfo instanceCreateInfo{
        .vulkanValidationLayers = {"VK_LAYER_KHRONOS_validation"},
        .vulkanExtensions = extensions,
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

    // Create swapchain
    auto vkSwapchain = renderer::Swapchain(vkPhysicalDevice, vkDevice, surface, 1280, 720);
}
