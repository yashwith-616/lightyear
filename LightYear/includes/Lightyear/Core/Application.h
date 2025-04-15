#pragma once

#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/Event.h"
#include "Lightyear/LightyearAPI.h"
#include "lypch.h"

namespace ly {

class Window;

class LIGHTYEAR_API Application {
public:
    Application();
    ~Application();

    virtual void Run();

    virtual void OnEvent(Event& event);

protected:
    virtual bool OnWindowClose(WindowCloseEvent& event);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};
}
