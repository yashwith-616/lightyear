#pragma once


#include <cassert>
#include <glfw/glfw3.h>
#include <string>

#include "LogScope.hpp"
#include "bootstrap/Instance.h"

class Window
{
public:
    Window()
    {
        glfwSetErrorCallback(&debugCallback);

        if (!glfwInit())
        {
            assert(false && "GLFW initialization failed");
        }
        m_isInitialized = true;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(1280, 720, "GLFW Window", nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            assert(false && "GLFW Window creation failed");
        }

        if (!glfwVulkanSupported())
        {
            glfwTerminate();
            assert(false && "GLFW Vulkan is not supported");
        }

        glfwSetWindowCloseCallback(m_window, &windowCloseCallback);
    }

    ~Window()
    {
        if (!m_isInitialized)
        {
            return;
        }
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    GLFWwindow* getHandle() { return m_window; }

    std::vector<std::string> getVulkanExtensions()
    {
        uint32_t count = 0;
        char const** extensions = glfwGetRequiredInstanceExtensions(&count);

        if (extensions == nullptr || count == 0)
        {
            return {};
        }
        return std::vector<std::string>(extensions, extensions + count);
    }

    VkSurfaceKHR createWindowSurface(ly::renderer::Instance const& instance)
    {
        VkSurfaceKHR surface;
        bool notValid = glfwCreateWindowSurface(*instance.getInstance(), m_window, nullptr, &surface);
        assert(!notValid && "Window surface could not be fetched");
        return surface;
    }

    bool shouldWindowClose() { return glfwWindowShouldClose(m_window); }

    void pollEvents() { return glfwPollEvents(); }

private:
    static void debugCallback(int error, char const* errMsg) { LOG(logger::Debug, fmt::runtime(errMsg)); }

    static void windowCloseCallback(GLFWwindow* window) { glfwSetWindowShouldClose(window, GLFW_TRUE); }

    GLFWwindow* m_window{nullptr};
    bool m_isInitialized = false;
};
