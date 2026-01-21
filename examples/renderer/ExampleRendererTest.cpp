#include <bootstrap/Instance.h>
#include <logging/Log.h>

#include <memory>

int main()
{
    namespace logger = ly::log;
    namespace renderer = ly::renderer;

    logger::LoggerConfig loggerConfig{
        .loggerName = "example_basic", .asyncQueueSize = 8192, .loggerThreadCount = 1, .isConsoleLoggingEnabled = true};

    auto logSystem = std::make_unique<logger::Log>();
    logSystem->init(loggerConfig);

    logger::Log::log<logger::Info>("Started Vulkan render test");
    renderer::InstanceCreateInfo instanceCreateInfo{
        .vulkanValidationLayers = {"VK_LAYER_KHRONOS_validation"},
        .vulkanExtensions =
            {vk::KHRSwapchainExtensionName,
             vk::KHRSpirv14ExtensionName,
             vk::KHRSynchronization2ExtensionName,
             vk::KHRCreateRenderpass2ExtensionName},
        .appName = "ExampleRendererTest",
        .appVersion = renderer::Version(1, 0, 0),
        .engineName = "No engine",
        .engineVersion = renderer::Version(1, 0, 0),
    };

    auto vkInstance = renderer::Instance(instanceCreateInfo);

    logger::Log::log<logger::Info>("Completed Vulkan render test");

    logSystem->shutdown();
}
