#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Log.h"
#include "Lightyear/Platform/Window.h"
#include "glad.h"

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
        glClearColor(0, 0, 0, 0.7);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event)
{
    // LY_CORE_INFO("OnEvent: {}", event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if (event.bIsHandled)
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
    m_Running = false;
    return true;
}

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
}

}
