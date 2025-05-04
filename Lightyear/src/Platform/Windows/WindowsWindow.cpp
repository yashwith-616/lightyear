#include "LightYear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/Platform/OpenGL/OpenGLContext.h"

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

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        LY_CORE_ASSERT(success, "Failed to initialize GLFW");
        s_GLFWInitialized = true;
    }

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(props.Width),
                                          static_cast<int>(props.Height),
                                          m_Data.Title.data(),
                                          nullptr,
                                          nullptr);

    m_Window  = Scope<GLFWwindow>(window);
    m_Context = MakeScope<OpenGLContext>(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window.get(), &m_Data);
    SetVSync(true);
}

void WindowsWindow::ShutDown() {
    glfwDestroyWindow(m_Window.get());
    glfwTerminate();
}

void WindowsWindow::SetupWindowCallbacks() {
    // Set windows callback
    glfwSetWindowSizeCallback(m_Window.get(), [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.Width        = width;
        data.Height       = height;

        WindowResizeEvent resizeEvent(data.Width, data.Height);
        data.EventCallback(resizeEvent);
    });

    glfwSetWindowCloseCallback(m_Window.get(), [](GLFWwindow* window) {
        DispatchWindowEvent(window, WindowCloseEvent());
    });

    // Set Mouse Events
    glfwSetMouseButtonCallback(
        m_Window.get(), [](GLFWwindow* window, int button, int action, int mods) {
            switch (action) {
                case GLFW_PRESS: {
                    DispatchWindowEvent(window, MouseButtonPressedEvent(button));
                    break;
                }
                case GLFW_RELEASE: {
                    DispatchWindowEvent(window, MouseButtonReleasedEvent(button));
                    break;
                }
            }
        });

    glfwSetScrollCallback(m_Window.get(), [](GLFWwindow* window, double xoffset, double yoffset) {
        DispatchWindowEvent(window, MouseScrolledEvent(xoffset, yoffset));
    });

    glfwSetCursorPosCallback(m_Window.get(), [](GLFWwindow* window, double xpos, double ypos) {
        DispatchWindowEvent(window, MouseMovedEvent(xpos, ypos));
    });

    // Set Keyboard Events
    glfwSetKeyCallback(
        m_Window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    DispatchWindowEvent(window, KeyPressedEvent(key, 0));
                    break;
                }
                case GLFW_RELEASE: {
                    DispatchWindowEvent(window, KeyReleasedEvent(key));
                    break;
                }
                case GLFW_REPEAT: {
                    DispatchWindowEvent(window, KeyPressedEvent(key, 1));
                    break;
                }
            }
        });

    glfwSetCharCallback(m_Window.get(), [](GLFWwindow* window, unsigned int keycode) {
        DispatchWindowEvent(window, KeyTypedEvent(keycode));
    });

    // Set Error Callbacks
    glfwSetErrorCallback(GLFWErrorCallback);
}

/**
 * @brief Template function to dispatch window event by fetching data from user window pointer
 * of glfw
 *
 * @tparam EventType The event type to be dispatched
 * @param window The pointer to glfwWindow
 * @param event The event to be dispatched
 */
template <typename EventType>
void DispatchWindowEvent(GLFWwindow* window, const EventType& event) {
    WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
    data.EventCallback(event);
}

}  // namespace ly
