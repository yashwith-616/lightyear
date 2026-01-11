#include "Lightyear/Core/Application.h"

#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Timestep.h"
#include "Lightyear/Editor/ImGUILayer.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly
{

Application::~Application() { renderer::Renderer::Shutdown(); }

void Application::create(Scope<Application> app)
{
    LY_CORE_ASSERT(!s_Instance, "Application already created!");
    s_Instance = std::move(app);
    s_Instance->Init();
}

Application& Application::get()
{
    LY_CORE_ASSERT(s_Instance, "Application is not initiated");
    return *s_Instance;
}

void Application::Init()
{
    Application::PushOverlay(MakeScope<ImGUILayer>());
    renderer::Renderer::Init();
}

void Application::Run()
{
    m_LastFrameTime = m_Window->GetTime();

    while (m_IsRunning)
    {
        const float currentTime = m_Window->GetTime();
        const Timestep timestep(currentTime - m_LastFrameTime);
        m_LastFrameTime = currentTime;

        for (const Scope<Layer>& layer : m_LayerStack)
        {
            layer->OnUpdate(timestep);
        }

        //---- Update Game Engine Editor
        ImGUILayer::BeginFrame();
        for (const Scope<Layer>& layer : m_LayerStack)
        {
            layer->OnEditorRender();
        }

        // End frame
        ImGUILayer::EndFrame();
        m_Window->OnUpdate();
    }

    for (const Scope<Layer>& layer : m_LayerStack)
        layer->OnDetach();
}

void Application::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& closeEvent) { return OnWindowClose(closeEvent); });

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(event);
        if (event.IsHandled())
        {
            break;
        }
    }
}

void Application::PushLayer(Scope<Layer> layer)
{
    layer->OnAttach();
    m_LayerStack.PushLayer(std::move(layer));
}

void Application::PushOverlay(Scope<Layer> overlay)
{
    overlay->OnAttach();
    m_LayerStack.PushOverlay(std::move(overlay));
}

Application::Application()
{
    LY_CORE_ASSERT(!s_Instance, "Application already exists!");
    m_Window = Window::Create();
    m_Window->Init();
    m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
}

bool Application::OnWindowClose(WindowCloseEvent& /*event*/)
{
    m_IsRunning = false;
    return true;
}

Scope<Application> Application::s_Instance = nullptr;

} // namespace ly
