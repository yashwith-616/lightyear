#pragma once

#include <lypch.h>
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;
class WindowCloseEvent;

class LIGHTYEAR_API Application {
public:
    Application();
    virtual ~Application() { LY_CORE_LOG(LogType::Trace, "Application is Destoryed!"); }

    virtual void Run();
    virtual void OnEvent(Event& event);

    virtual void PushLayer(Scope<Layer> layer);
    virtual void PushOverlay(Scope<Layer> overlay);

    inline static Application& Get() {
        LY_CORE_ASSERT(s_Application, "Application is not initiated");
        return *s_Application;
    }

    inline static void Destroy() { s_Application.reset(); }

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
