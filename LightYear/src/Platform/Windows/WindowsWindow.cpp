#include "LightYear/Platform/Windows/WindowsWindow.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "glfw/glfw3.h"

namespace ly {

static bool s_GLFWInitialized { false };

Window* Window::Create(const WindowProps& props)
{
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
}

void WindowsWindow::OnUpdate()
{
    glClearColor(0, 0, 0, 0.7);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void WindowsWindow::SetVSync(bool isEnabled)
{
    glfwSwapInterval(isEnabled ? 1 : 0);
    m_Data.VSync = isEnabled;
}

bool WindowsWindow::IsVSync() const
{
    return m_Data.VSync;
}

void WindowsWindow::Init(const WindowProps& props)
{
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    if (!s_GLFWInitialized) {
        int success = glfwInit();
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow(
        static_cast<int>(props.Width),
        static_cast<int>(props.Height),
        m_Data.Title.c_str(),
        nullptr, nullptr);

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Set windows callback
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));
        data.Width = width;
        data.Height = height;

        WindowResizeEvent resizeEvent(data.Width, data.Height);
        data.EventCallback(resizeEvent);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowsData& data = *static_cast<WindowsData*>(glfwGetWindowUserPointer(window));

        WindowCloseEvent windowCloseEvent {};
        data.EventCallback(windowCloseEvent);
    });
}

void WindowsWindow::ShutDown()
{
    glfwDestroyWindow(m_Window);
}

}
