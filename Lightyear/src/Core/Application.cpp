#include "Lightyear/Core/Application.h"
#include "../../includes/Lightyear/Common/Log.h"
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Timestep.h"
#include "Lightyear/Editor/ImGUILayer.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/EditorEvent.h"
#include "Lightyear/Events/Event.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly {

Scope<Application> Application::s_Application = nullptr;

Application::Application() {
    LY_CORE_ASSERT(!s_Application, "Application already exists!");
    m_Window = Window::Create();
    m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
}

Application::~Application() {
    renderer::Renderer::Shutdown();
}

void Application::Init() {
    renderer::Renderer::Init();
    m_ImGUILayer = MakeScope<ImGUILayer>();
}

void Application::Run() {
    m_LastFrameTime = m_Window->GetTime();

    while (m_Running) {
        const float currentTime = m_Window->GetTime();
        const Timestep timestep(currentTime - m_LastFrameTime);
        m_LastFrameTime = currentTime;

        for (const Scope<Layer>& layer : m_LayerStack) {
            layer->OnUpdate(timestep);
        }

        //---- Update Game Engine Editor
        m_ImGUILayer->Begin();
        for (const Scope<Layer>& layer : m_LayerStack) {
            layer->OnEditorRender();
            m_ImGUILayer->OnEditorRender();
        }
        m_ImGUILayer->End();
        m_Window->OnUpdate();
    }
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& closeEvent) { return OnWindowClose(closeEvent); });

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if (event.IsHandled()) {
            break;
        }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& /*event*/) {
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
