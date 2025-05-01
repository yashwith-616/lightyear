#include "Lightyear/Core/Application.h"
#include <glad.h>
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Log.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"

namespace ly {

Application* Application::s_Application = nullptr;

Application::Application() {
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    s_Application = this;
}

Application::~Application() {}

void Application::Run() {
    while (m_Running) {
        glClearColor(0, 0, 0, 0.7);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    // LY_CORE_INFO("OnEvent: {}", event);
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

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

}  // namespace ly
