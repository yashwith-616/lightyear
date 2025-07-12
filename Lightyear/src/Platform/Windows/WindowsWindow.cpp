#include "Lightyear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/OpenGL/OpenGLContext.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

LY_DISABLE_WARNINGS_PUSH
#include <GLFW/glfw3.h>
LY_DISABLE_WARNINGS_PUSH

namespace {
void GLFWErrorCallback(int error, const char* description) {
    LY_CORE_LOG(ly::LogType::Error, "GLFW Error ({0}): {1}", error, description);
}
}

namespace ly {

Scope<Window> Window::Create(const WindowProps& props) {
    return MakeScope<WindowsWindow>(props);
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::Init(const WindowProps& props) {
    if (!m_bIsGLFWInitialized) {
        const int success = glfwInit();
        LY_CORE_ASSERT(success, "Failed to initialize GLFW");
        m_bIsGLFWInitialized = true;
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    if (renderer::Renderer::GetAPI() == renderer::RendererAPI::API::OpenGL) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef LY_GRAPHICS_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    }

    m_Window = glfwCreateWindow(
        static_cast<int>(props.Size.x), static_cast<int>(props.Size.y), m_Data.Title.data(), nullptr, nullptr);
    LY_CORE_ASSERT(m_Window != nullptr, "GLFW window initialization failed!");

    m_Context = renderer::RendererContext::Create(reinterpret_cast<void*>(m_Window));
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);
}

void WindowsWindow::ShutDown() {
    LY_CORE_ASSERT(m_Window != nullptr, "GLFWWindow is nullptr");

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowsWindow::SetVSync(bool isEnabled) {
    glfwSwapInterval(isEnabled ? GLFW_TRUE : GLFW_FALSE);
    m_Data.VSync = isEnabled;
}

float WindowsWindow::GetTime() const {
    return static_cast<float>(glfwGetTime());
}

// NOLINTBEGIN
void WindowsWindow::SetupWindowCallbacks() {
    // Set windows callback
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.WindowSize = glm::vec2(width, height);

        WindowResizeEvent resizeEvent(data.WindowSize.x, data.WindowSize.y);
        data.EventCallback(resizeEvent);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent windowCloseEvent{};
        data.EventCallback(windowCloseEvent);
    });

    // Set Mouse Events
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent pressedEvent(button);
                data.EventCallback(pressedEvent);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent releasedEvent(button);
                data.EventCallback(releasedEvent);
                break;
            }
            default: break;
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent scrolledEvent(xoffset, yoffset);
        data.EventCallback(scrolledEvent);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent mouseMoveEvent(xpos, ypos);
        data.EventCallback(mouseMoveEvent);
    });

    // Set Keyboard Events
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent pressedEvent(key, 0);
                data.EventCallback(pressedEvent);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent releasedEvent(key);
                data.EventCallback(releasedEvent);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent repeatEvent(key, 1);
                data.EventCallback(repeatEvent);
                break;
            }
            default : break;
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        KeyTypedEvent typedEvent(keycode);
        data.EventCallback(typedEvent);
    });
}
// NOLINTEND

}  // namespace ly
