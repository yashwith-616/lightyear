#pragma once

#include "LightYear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class WindowResizeEvent;

class LIGHTYEAR_API ImGuiLayer : public Layer {
public:
    ImGuiLayer();
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
