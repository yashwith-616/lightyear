#pragma once

#include "LightYear/Core/Layer.h"
#include "Lightyear/Events/ApplicationEvent.h"
#include "Lightyear/Events/KeyEvent.h"
#include "Lightyear/Events/MouseEvent.h"
#include "Lightyear/LightyearAPI.h"

namespace ly {

class LIGHTYEAR_API ImGuiLayer : public Layer {
public:
    ImGuiLayer() : Layer("ImguiLayer") {}
    ~ImGuiLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnEvent(Event& event);

protected:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& event);
    bool OnMouseMovedEvent(MouseMovedEvent& event);
    bool OnMouseScrolledEvent(MouseScrolledEvent& event);

    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnKeyTypedEvent(KeyTypedEvent& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);

private:
    float m_Time{ 0.f };
};
}  // namespace ly
