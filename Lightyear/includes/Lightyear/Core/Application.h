#pragma once

#include <lypch.h>
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class Layer;
class Event;
class WindowCloseEvent;
class Window;

class LIGHTYEAR_API Application {
public:
    Application();
    ~Application() = default;

    virtual void Run();
    virtual void OnEvent(Event& event);

    virtual void PushLayer(Layer* layer);
    virtual void PushOverlay(Layer* layer);

    inline static Application& Get() {
        LY_CORE_ASSERT(s_Application, "Application is not initiated");
        return *s_Application; 
    }
    inline static void SetApplication(Scope<Application> application) {
        s_Application = std::move(application);
    }

    inline Window& GetWindow() const { return *m_Window; }

protected:
    virtual bool OnWindowClose(WindowCloseEvent& event);

private:
    Scope<Window> m_Window;

    bool m_Running{ true };
    float m_Frametime{ DEFAULT_FRAMETIME };
    float m_LastFrameTime{ 0.f };

    LayerStack m_LayerStack;
    static Scope<Application> s_Application;
};
}  // namespace ly
