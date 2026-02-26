#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace ly::renderer
{

struct Version
{
    int32_t major{};
    int32_t minor{};
    int32_t patch{};

    Version() = default;

    Version(int32_t major, int32_t minor, int32_t patch) : major(major), minor(minor), patch(patch)
    {
        assert(major >= 0 && minor >= 0 && patch >= 0 && "Version numbers must always be positive");
    }
};

struct InstanceCreateInfo
{
    std::vector<std::string> vulkanValidationLayers;
    std::vector<std::string> vulkanExtensions;

    std::string appName;
    Version appVersion;

    std::string engineName;
    Version engineVersion;
};

class Instance
{
public:
    Instance(InstanceCreateInfo const& instanceCreateInfo);
    ~Instance() = default;

    vk::raii::Instance const& getInstance() const;
    std::vector<char const*> const& getVulkanExtensions() const;

private:
    vk::raii::Instance createInstance(InstanceCreateInfo const& instanceCreateInfo);

    std::vector<std::string_view> resolveAllValidationLayers(std::vector<std::string> const& requestedLayers) const;

    void resolveAllExtensions(std::vector<std::string> const& requestedExtensions);

    vk::raii::Context m_context{};
    vk::raii::Instance m_instance;

    std::vector<std::string_view> m_vulkanExtensions{};
};

} // namespace ly::renderer
