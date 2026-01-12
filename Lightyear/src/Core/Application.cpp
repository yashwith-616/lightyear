#include "Lightyear/Core/Application.h"
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Timestep.h"
#include "Lightyear/Editor/ImGUILayer.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"
#include "Lightyear/Renderer/Abstract/Renderer.h"

namespace ly {

Application::~Application() {
    renderer::Renderer::shutdown();
}

void Application::create(scope<Application> app) {
    LY_CORE_ASSERT(!m_instance, "Application already created!");
    m_instance = std::move(app);
    m_instance->init();
}

Application& Application::get() {
    LY_CORE_ASSERT(m_instance, "Application is not initiated");
    return *m_instance;
}

void Application::init() {
    Application::pushOverlay(makeScope<ImGuiLayer>());
    renderer::Renderer::init();
}

void Application::run() {
    m_lastFrameTime = m_window->getTime();

    while (m_isRunning) {
        float const currentTime = m_window->getTime();
        Timestep const timestep(currentTime - m_lastFrameTime);
        m_lastFrameTime = currentTime;

        for (scope<Layer> const& layer : m_layerStack) {
            layer->onUpdate(timestep);
        }

        //---- Update Game Engine Editor
        ImGuiLayer::beginFrame();
        for (scope<Layer> const& layer : m_layerStack) {
            layer->onEditorRender();
        }
        ImGuiLayer::endFrame();
        m_window->onUpdate();
    }

    for (scope<Layer> const& layer : m_layerStack) {
        layer->onDetach();
    }
}

void Application::onEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& closeEvent) { return onWindowClose(closeEvent); });

    for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
        (*--it)->onEvent(event);
        if (event.isHandled()) {
            break;
        }
    }
}

void Application::pushLayer(scope<Layer> layer) {
    layer->onAttach();
    m_layerStack.pushLayer(std::move(layer));
}

void Application::pushOverlay(scope<Layer> overlay) {
    overlay->onAttach();
    m_layerStack.pushOverlay(std::move(overlay));
}

Application::Application() {
    LY_CORE_ASSERT(!m_instance, "Application already exists!");
    m_window = Window::create();
    m_window->init();
    m_window->setEventCallback([this](Event& event) { onEvent(event); });
}

bool Application::onWindowClose(WindowCloseEvent& /*event*/) {
    m_isRunning = false;
    return true;
}

scope<Application> Application::m_instance = nullptr;

}  // namespace ly
