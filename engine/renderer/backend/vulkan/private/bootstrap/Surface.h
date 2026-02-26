#pragma once

#include <vulkan/vulkan.h>

namespace ly::renderer
{

class Surface
{
public:
    Surface(VkSurfaceKHR surface) : m_surface(surface) {}

    VkSurfaceKHR getHandle() const { return m_surface; }

private:
    VkSurfaceKHR m_surface;
};

} // namespace ly::renderer
