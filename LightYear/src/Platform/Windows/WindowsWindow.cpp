#include "LightYear/Platform/Windows/WindowsWindow.h"
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

    m_Window = glfwCreateWindow(static_cast<int>(props.Width),
        static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);
}

void WindowsWindow::ShutDown()
{
    glfwDestroyWindow(m_Window);
}

}
