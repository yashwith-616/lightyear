#include "Instance.h"

#include <expected>

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

Instance::Instance(InstanceCreateInfo const& instanceCreateInfo) : m_instance(createInstance(instanceCreateInfo)) {}

vk::raii::Instance const& Instance::getInstance() const { return m_instance; }

std::vector<char const*> const& Instance::getVulkanExtensions() const { return toCArray(m_vulkanExtensions); }

vk::raii::Instance Instance::createInstance(InstanceCreateInfo const& instanceCreateInfo)
{
    vk::ApplicationInfo appInfo{
        .pApplicationName = instanceCreateInfo.appName.c_str(),
        .applicationVersion = makeVersion(instanceCreateInfo.appVersion),
        .pEngineName = instanceCreateInfo.engineName.c_str(),
        .engineVersion = makeVersion(instanceCreateInfo.engineVersion),
        .apiVersion = vk::ApiVersion14};

    std::vector<std::string_view> validationLayers =
        resolveAllValidationLayers(instanceCreateInfo.vulkanValidationLayers);
    resolveAllExtensions(instanceCreateInfo.vulkanExtensions);


    auto rawValidationLayers = toCArray(validationLayers);
    auto layerCount = static_cast<uint32_t>(validationLayers.size());

    auto rawExtensions = toCArray(m_vulkanExtensions);
    auto extensionsCount = static_cast<uint32_t>(m_vulkanExtensions.size());

    vk::InstanceCreateInfo createInfo{
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = layerCount,
        .ppEnabledLayerNames = rawValidationLayers.data(),
        .enabledExtensionCount = extensionsCount,
        .ppEnabledExtensionNames = rawExtensions.data()};

    std::expected<vk::raii::Instance, vk::Result> instanceExpected = m_context.createInstance(createInfo);
    assert(instanceExpected.has_value() && "Instance could not be created");

    return std::move(instanceExpected.value());
}

/// \brief Validate if required validation layers are available in the vulkan context
///
/// \param requestedLayers All vulkan validation layers to be enabled
std::vector<std::string_view>
    Instance::resolveAllValidationLayers(std::vector<std::string> const& requestedLayers) const
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
    m_vulkanExtensions.clear();
    std::ranges::transform(
        requestedExtensions, std::back_inserter(m_vulkanExtensions), [](auto const& s) { return s.c_str(); });

#if ENABLE_VK_VALIDATION
    m_vulkanExtensions.push_back(vk::EXTDebugUtilsExtensionName);
#endif
}

} // namespace ly::renderer
