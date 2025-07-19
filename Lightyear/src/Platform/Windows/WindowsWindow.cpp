#include "Lightyear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/OpenGL/Renderer/Core/OpenGLContext.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

LY_DISABLE_WARNINGS_PUSH
#include <GLFW/glfw3.h>
LY_DISABLE_WARNINGS_PUSH

namespace {
void GLFWErrorCallback(int error, const char* description) {
    LY_CORE_LOG(ly::LogType::Error, "GLFW Error ({0}): {1}", error, description);
}
}  // namespace

namespace ly {

Scope<Window> Window::Create(const WindowProps& props) {
    return MakeScope<WindowsWindow>(props);
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::Init() {
    if (!m_IsGLFWInitialized) {
        const int success = glfwInit();
        LY_CORE_ASSERT(success, "Failed to initialize GLFW");
        m_IsGLFWInitialized = true;
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    SetupGLFWWindowHints();

    // NOLINTNEXTLINE
    m_Window = glfwCreateWindow(narrow_cast<int>(m_Data.WindowSize.x),
                                narrow_cast<int>(m_Data.WindowSize.y),
                                m_Data.Title.c_str(),
                                nullptr,
                                nullptr);
    LY_CORE_ASSERT(m_Window != nullptr, "GLFW window initialization failed!");

    m_Context = renderer::RendererContext::Create(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    SetupWindowCallbacks();
}

void WindowsWindow::ShutDown() {
    LY_CORE_ASSERT(m_Window != nullptr, "GLFWWindow is nullptr");

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowsWindow::SetVSync(bool enable) {
    glfwSwapInterval(enable ? GLFW_TRUE : GLFW_FALSE);
    m_Data.IsVSyncEnabled = enable;
}

float WindowsWindow::GetTime() const {
    return static_cast<float>(glfwGetTime());
}

/**
 * Window Hint Configuration for GLFW:
 * GLFW window hints (glfwWindowHint) must be set *before* calling glfwCreateWindow. This function applies the
 * necessary hints to ensure the created GLFW window is compatible with the selected rendering API backend.
 */
void WindowsWindow::SetupGLFWWindowHints() {
    using API = renderer::RendererAPI::API;

    switch (renderer::Renderer::GetAPI()) {
        case API::OpenGL:
            [[likely]] {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef LY_OPENGL_DEBUG
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
                return;
            }
        default:
            [[unlikely]] LY_CORE_ASSERT(false, "Unknown renderer API");
    }
}

// NOLINTBEGIN
void WindowsWindow::SetupWindowCallbacks() {
    // Set windows callback
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.WindowSize   = glm::uvec2(width, height);

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
            default:
                break;
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
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
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
            default:
                break;
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
