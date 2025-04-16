#pragma once

#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"
#include "Lightyear/Core/Layer.h"
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/LightyearAPI.h"
#include "lypch.h"

class Layer;

namespace ly {

class Window;

class LIGHTYEAR_API Application {
public:
    Application();
    ~Application();

    virtual void Run();

    virtual void OnEvent(Event& event);
    
    virtual void PushLayer(Layer* layer);
    virtual void PushOverlay(Layer* layer);

protected:
    virtual bool OnWindowClose(WindowCloseEvent& event);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    LayerStack m_LayerStack;
    
};
}
