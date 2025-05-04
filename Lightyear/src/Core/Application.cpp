#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Log.h"
#include "Lightyear/Core/Timestep.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"

#include <glad.h>

namespace ly {

Scope<Application> Application::s_Application = nullptr;

Application::Application() {
    m_Window = Window::Create();
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}

void Application::Run() {
    m_LastFrameTime = m_Window->GetTime();

    while (m_Running) {
        float currentTime = m_Window->GetTime();
        Timestep timestep(currentTime - m_LastFrameTime);
        m_LastFrameTime = currentTime;

        for (const Scope<Layer>& layer : m_LayerStack) {
            layer->OnUpdate(timestep);
        }

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(
        [this](WindowCloseEvent& event) { return OnWindowClose(event); });

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if (event.bIsHandled) break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

void Application::PushLayer(Scope<Layer> layer) {
    layer->OnAttach();
    m_LayerStack.PushLayer(std::move(layer));
}

void Application::PushOverlay(Scope<Layer> overlay) {
    overlay->OnAttach();
    m_LayerStack.PushOverlay(std::move(overlay));
}

}  // namespace ly
