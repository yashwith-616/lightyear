#pragma once

#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Window.h"
#include "Lightyear/LightyearCore.h"

#include <lypch.h>

namespace ly {

class Event;
class WindowCloseEvent;
class ImGUILayer;

class LIGHTYEAR_API Application {
public:
    Application();
    virtual ~Application();

    virtual void Init();
    virtual void Run();
    virtual void OnEvent(Event& event);

    virtual void PushLayer(Scope<Layer> layer);
    virtual void PushOverlay(Scope<Layer> overlay);

    inline static Application& Get() {
        LY_CORE_ASSERT(s_Application, "Application is not initiated");
        return *s_Application;
    }

    static void Create(Scope<Application> app) {
        LY_CORE_ASSERT(!s_Application, "Application already created!");
        s_Application = std::move(app);
        s_Application->Init();
    }

    inline Window& GetWindow() const { return *m_Window; }

protected:
    virtual bool OnWindowClose(WindowCloseEvent& event);

private:
    Scope<Window> m_Window{};
    LayerStack m_LayerStack{};

    bool m_Running{ true };
    float m_Frametime{ DEFAULT_FRAMETIME };
    float m_LastFrameTime{ 0.f };

    ImGUILayer* m_ImGUILayer{ nullptr };

private:
    static Scope<Application> s_Application;
};

}  // namespace ly
