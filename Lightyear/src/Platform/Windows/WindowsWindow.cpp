#include "LightYear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/OpenGL/OpenGLContext.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

#include <GLFW/glfw3.h>

namespace ly {

static bool s_GLFWInitialized{ false };

static void GLFWErrorCallback(int error, const char* description) {
    LY_CORE_LOG(LogType::Error, "GLFW Error ({0}): {1}", error, description);
}

Scope<Window> Window::Create(const WindowProps& props) {
    return MakeScope<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
    SetupWindowCallbacks();
}

WindowsWindow::~WindowsWindow() {
    LY_CORE_LOG(LogType::Trace, "Destroying Windows!");
    ShutDown();
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void WindowsWindow::SetVSync(bool isEnabled) {
    glfwSwapInterval(isEnabled ? GLFW_TRUE : GLFW_FALSE);
    m_Data.VSync = isEnabled;
}

bool WindowsWindow::IsVSync() const {
    return m_Data.VSync;
}

float WindowsWindow::GetTime() const {
    return static_cast<float>(glfwGetTime());
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    LY_CORE_LOG(
        LogType::Info, "Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (renderer::Renderer::GetAPI() == renderer::RendererAPI::API::OpenGL) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef LY_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    }

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        LY_CORE_ASSERT(success, "Failed to initialize GLFW");
        s_GLFWInitialized = true;

        // Set Error Callbacks
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    m_Window = glfwCreateWindow(static_cast<int>(props.Width),
                                static_cast<int>(props.Height),
                                m_Data.Title.data(),
                                nullptr,
                                nullptr);
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

void WindowsWindow::SetupWindowCallbacks() {
    // Set windows callback
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.Width        = width;
        data.Height       = height;

        WindowResizeEvent resizeEvent(data.Width, data.Height);
        data.EventCallback(resizeEvent);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent windowCloseEvent{};
        data.EventCallback(windowCloseEvent);
    });

    // Set Mouse Events
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

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
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent scrolledEvent(xoffset, yoffset);
        data.EventCallback(scrolledEvent);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent mouseMoveEvent(xpos, ypos);
        data.EventCallback(mouseMoveEvent);
    });

    // Set Keyboard Events
    glfwSetKeyCallback(
        m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

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
            }
        });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        KeyTypedEvent typedEvent(keycode);
        data.EventCallback(typedEvent);
    });
}

}  // namespace ly
