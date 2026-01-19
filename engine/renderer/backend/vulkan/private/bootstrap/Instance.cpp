#include "Instance.h"

namespace
{

uint32_t makeVersion(ly::renderer::Version version)
{
    return VK_MAKE_VERSION(version.major, version.minor, version.patch);
}

std::vector<char const*> toCArray(std::span<std::string_view const> views)
{
    std::vector<char const*> result(views.size(), nullptr);
    std::ranges::transform(views, std::back_inserter(result), [](std::string_view sv) {
        return sv.data(); // NOT c_str()
    });

    return result;
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

} // namespace

namespace ly::renderer
{

VkInstance::VkInstance(InstanceCreateInfo const& instanceCreateInfo) : m_instance(createInstance(instanceCreateInfo)) {}

vk::raii::Instance const& VkInstance::getInstance() const { return m_instance; }

vk::raii::Instance VkInstance::createInstance(InstanceCreateInfo const& instanceCreateInfo)
{
    vk::ApplicationInfo appInfo{
        .pApplicationName = instanceCreateInfo.appName.c_str(),
        .applicationVersion = makeVersion(instanceCreateInfo.appVersion),
        .pEngineName = instanceCreateInfo.engineName.c_str(),
        .engineVersion = makeVersion(instanceCreateInfo.engineVersion),
        .apiVersion = vk::ApiVersion14};

    std::vector<std::string_view> validationLayers =
        resolveAllValidationLayers(instanceCreateInfo.vulkanValidationLayers);
    std::vector<std::string_view> extensions = resolveAllExtensions(instanceCreateInfo.vulkanExtensions);


    auto rawValidationLayers = toCArray(validationLayers);
    auto layerCount = static_cast<uint32_t>(validationLayers.size());

    auto rawExtensions = toCArray(extensions);
    auto extensionsCount = static_cast<uint32_t>(extensions.size());

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = layerCount,
        .ppEnabledLayerNames = rawValidationLayers.data(),
        .enabledExtensionCount = extensionsCount,
        .ppEnabledExtensionNames = rawExtensions.data()};
    return vk::raii::Instance(m_context, createInfo);
}

/// \brief Validate if required validation layers are available in the vulkan context
///
/// \param requestedLayers All vulkan validation layers to be enabled
std::vector<std::string_view>
    VkInstance::resolveAllValidationLayers(std::vector<std::string> const& requestedLayers) const
{
    std::vector<std::string_view> validationLayers{};
#if ENABLE_VK_VALIDATION
    for (auto& layers : requestedLayers)
    {
        validationLayers.emplace_back(layers.c_str());
    }

    setupVkValidationEnvironment();
#endif

    auto availableLayers = m_context.enumerateInstanceLayerProperties();
    bool isAnyRequiredLayerMissing = std::ranges::any_of(requestedLayers, [&](auto& requestedLayer) {
        return std::ranges::none_of(availableLayers, [&](vk::LayerProperties const& layer) {
            return strcmp(layer.layerName, requestedLayer) == 0;
        });
    });

    assert(!isAnyRequiredLayerMissing && "Required layers may be missing");
    return validationLayers;
}

/// \brief Resolve all extensions required to be part of the vulkan instance
///
/// \param requestedExtensions the requested extensions
/// \return All vulkan extensions
std::vector<std::string_view>
    VkInstance::resolveAllExtensions(std::vector<std::string> const& requestedExtensions) const
{
    std::vector<std::string_view> extensions(requestedExtensions.size(), nullptr);
    std::ranges::transform(
        requestedExtensions, std::back_inserter(extensions), [](auto const& s) { return s.c_str(); });

#if ENABLE_VK_VALIDATION
    extensions.push_back(vk::EXTDebugUtilsExtensionName);
#endif

    return extensions;
}

} // namespace ly::renderer
