#include "Instance.h"

#include <expected>
#include <iostream>

namespace
{

uint32_t makeVersion(ly::renderer::Version version)
{
    return VK_MAKE_VERSION(version.major, version.minor, version.patch);
}

void setupVkValidationEnvironment()
{
#if ENABLE_VK_VALIDATION
#if defined(_WIN32)
    _putenv_s("VK_LAYER_PATH", VALIDATION_LAYER_PATH);
#else
    setenv("VK_LAYER_PATH", VALIDATION_LAYER_PATH, 1);
#endif
#endif
}

void validateExtensions(
    std::vector<std::string> const& requested, std::vector<vk::ExtensionProperties> const& available)
{
    std::vector<std::string> missingExtensions;

    for (auto const& req : requested)
    {
        // Look for the requested string in the list of available properties
        auto it = std::ranges::find_if(available, [&](const vk::ExtensionProperties& prop) {
            return std::string_view(prop.extensionName) == req;
        });

        if (it == available.end())
        {
            missingExtensions.push_back(req);
        }
    }

    if (!missingExtensions.empty())
    {
        std::cerr << "\n--- VULKAN EXTENSION ERROR ---\n";
        for (auto const& missing : missingExtensions)
        {
            std::cerr << "[!] Not Supported: " << missing << "\n";
        }
        std::cerr << "------------------------------\n" << std::endl;
        assert(
            missingExtensions.empty() &&
            "One or more requested Vulkan extensions are not supported by the driver/hardware.");
    }
}

} // namespace

namespace ly::renderer
{

Instance::Instance(InstanceCreateInfo const& instanceCreateInfo) { m_instance = createInstance(instanceCreateInfo); }

vk::raii::Instance const& Instance::getInstance() const { return m_instance; }

std::vector<std::string> const& Instance::getVulkanExtensions() const { return m_deviceExtensions; }

vk::raii::Instance Instance::createInstance(InstanceCreateInfo const& instanceCreateInfo)
{
    vk::ApplicationInfo appInfo{
        .pApplicationName = instanceCreateInfo.appName.c_str(),
        .applicationVersion = makeVersion(instanceCreateInfo.appVersion),
        .pEngineName = instanceCreateInfo.engineName.c_str(),
        .engineVersion = makeVersion(instanceCreateInfo.engineVersion),
        .apiVersion = vk::ApiVersion14};

    m_deviceExtensions = instanceCreateInfo.deviceExtensions;

    std::vector<char const*> vkValidationLayers;
    std::vector<std::string> validationLayers = resolveAllValidationLayers(instanceCreateInfo.vulkanValidationLayers);
    vkValidationLayers.reserve(validationLayers.size());
    for (auto const& str : validationLayers)
    {
        vkValidationLayers.push_back(str.c_str());
    }

    std::vector<char const*> rawExtensions;
    resolveAllExtensions(instanceCreateInfo.vulkanExtensions);
    rawExtensions.reserve(m_vulkanExtensions.size());
    for (auto const& str : m_vulkanExtensions)
    {
        rawExtensions.push_back(str.c_str());
    }

    auto layerCount = static_cast<uint32_t>(vkValidationLayers.size());
    auto extensionsCount = static_cast<uint32_t>(rawExtensions.size());
    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = layerCount,
        .ppEnabledLayerNames = vkValidationLayers.data(),
        .enabledExtensionCount = extensionsCount,
        .ppEnabledExtensionNames = rawExtensions.data()};

    std::expected<vk::raii::Instance, vk::Result> instanceExpected = m_context.createInstance(createInfo);
    assert(instanceExpected.has_value() && "Instance could not be created");
    return std::move(instanceExpected.value());
}

/// \brief Validate if required validation layers are available in the vulkan context
///
/// \param requestedLayers All vulkan validation layers to be enabled
std::vector<std::string> Instance::resolveAllValidationLayers(std::vector<std::string> const& requestedLayers) const
{
    std::vector<std::string> validationLayers{};
#if ENABLE_VK_VALIDATION
    for (auto& layers : requestedLayers)
    {
        validationLayers.emplace_back(layers.c_str());
    }

    setupVkValidationEnvironment();
#endif

    auto availableLayers = m_context.enumerateInstanceLayerProperties();
    bool isAnyRequiredLayerMissing = std::ranges::any_of(requestedLayers, [&](std::string const& requestedLayer) {
        return std::ranges::none_of(availableLayers, [&](vk::LayerProperties const& layer) {
            return std::strcmp(layer.layerName, requestedLayer.c_str()) == 0;
        });
    });

    assert(!isAnyRequiredLayerMissing && "Required layers may be missing");
    return validationLayers;
}

/// \brief Resolve all extensions required to be part of the vulkan instance
///
/// \param requestedExtensions the requested extensions
/// \return All vulkan extensions
void Instance::resolveAllExtensions(std::vector<std::string> const& requestedExtensions)
{
    auto available = vk::enumerateInstanceExtensionProperties();
    validateExtensions(requestedExtensions, available.value);
    m_vulkanExtensions = requestedExtensions;

#if ENABLE_VK_VALIDATION
    m_vulkanExtensions.push_back(vk::EXTDebugUtilsExtensionName);
#endif
}

} // namespace ly::renderer
