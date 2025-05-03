#pragma once

#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/LightyearCore.h"
#include "lypch.h"

namespace ly {

class Layer;
class Event;
class WindowCloseEvent;
class Window;

class LIGHTYEAR_API Application {
public:
    Application();
    ~Application();

    virtual void Run();

    virtual void OnEvent(Event& event);

    virtual void PushLayer(Layer* layer);
    virtual void PushOverlay(Layer* layer);

    inline static Application& Get() { return *s_Application; }
    inline Window& GetWindow() const { return *m_Window; }

protected:
    virtual bool OnWindowClose(WindowCloseEvent& event);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    float m_Frametime{ 1 / 60.f };
    float m_LastFrameTime{ 0.f };

    LayerStack m_LayerStack;

    static Application* s_Application;
};
}  // namespace ly
