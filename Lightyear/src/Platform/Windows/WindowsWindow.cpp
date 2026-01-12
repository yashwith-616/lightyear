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
void glfwErrorCallback(int error, char const* description) {
    LY_CORE_LOG(ly::LogType::Error, "GLFW Error ({0}): {1}", error, description);
}
}  // namespace

namespace ly {

scope<Window> Window::create(WindowProps const& props) {
    return makeScope<WindowsWindow>(props);
}

void WindowsWindow::onUpdate() {
    glfwPollEvents();
    m_context->swapBuffers();
}

void WindowsWindow::setVSync(bool enable) {
    glfwSwapInterval(enable ? GLFW_TRUE : GLFW_FALSE);
    m_data.isVSyncEnabled = enable;
}

float WindowsWindow::getTime() const {
    return static_cast<float>(glfwGetTime());
}

void WindowsWindow::init() {
    if (!m_isGlfwInitialized) {
        int const success = glfwInit();
        LY_CORE_ASSERT(success, "Failed to initialize GLFW");
        m_isGlfwInitialized = true;
        glfwSetErrorCallback(glfwErrorCallback);
    }

    setupGlfwWindowHints();

    // NOLINTNEXTLINE
    m_window = glfwCreateWindow(narrowCast<int>(m_data.windowSize.x),
                                narrowCast<int>(m_data.windowSize.y),
                                m_data.title.c_str(),
                                nullptr,
                                nullptr);
    LY_CORE_ASSERT(m_window != nullptr, "GLFW window initialization failed!");

    m_context = renderer::RendererContext::create(m_window);
    m_context->init();

    glfwSetWindowUserPointer(m_window, &m_data);
    setVSync(true);

    setupWindowCallbacks();
}

void WindowsWindow::shutDown() {
    LY_CORE_ASSERT(m_window != nullptr, "GLFWWindow is nullptr");

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

// NOLINTBEGIN
void WindowsWindow::setupWindowCallbacks() {
    // Set windows callback
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.windowSize   = glm::uvec2(width, height);

        WindowResizeEvent resizeEvent(data.windowSize.x, data.windowSize.y);
        data.eventCallback(resizeEvent);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent windowCloseEvent{};
        data.eventCallback(windowCloseEvent);
    });

    // Set Mouse Events
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent pressedEvent(button);
                data.eventCallback(pressedEvent);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent releasedEvent(button);
                data.eventCallback(releasedEvent);
                break;
            }
            default:
                break;
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent scrolledEvent(xoffset, yoffset);
        data.eventCallback(scrolledEvent);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent mouseMoveEvent(xpos, ypos);
        data.eventCallback(mouseMoveEvent);
    });

    // Set Keyboard Events
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent pressedEvent(key, 0);
                data.eventCallback(pressedEvent);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent releasedEvent(key);
                data.eventCallback(releasedEvent);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent repeatEvent(key, 1);
                data.eventCallback(repeatEvent);
                break;
            }
            default:
                break;
        }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
        const WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        KeyTypedEvent typedEvent(keycode);
        data.eventCallback(typedEvent);
    });
}

/**
 * Window Hint Configuration for GLFW:
 * GLFW window hints (glfwWindowHint) must be set *before* calling glfwCreateWindow. This function applies the
 * necessary hints to ensure the created GLFW window is compatible with the selected rendering API backend.
 */
void WindowsWindow::setupGlfwWindowHints() {
    using API = renderer::RendererApi::Api;

    switch (renderer::Renderer::getApi()) {
        case API::OpenGl:
            [[likely]] {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, k_openglMajorVersion);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, k_openglMinorVersion);
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
// NOLINTEND

}  // namespace ly
