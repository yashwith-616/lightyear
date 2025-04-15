#include "Lightyear/Core/Application.h"
#include "Lightyear/Platform/Window.h"
#include "Lightyear/Core/Log.h"
#include "iostream"

namespace ly {

Application::Application()
{
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

Application::~Application()
{
}

void Application::Run()
{
    while (m_Running) {
        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    std::cout << "OnEvent: " << event.ToString() << '\n';

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    std::cout << "WindowClose: " << event.ToString() << '\n';
    m_Running = false;
    return true;
}

}
