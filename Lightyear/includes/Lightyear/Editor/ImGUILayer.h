#pragma once

#include "LightYear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

enum ImGuiKey : int;

namespace ly {

class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class WindowResizeEvent;
class EditorUpdateBeginEvent;
class EditorUpdateEndEvent;

class LIGHTYEAR_API ImGUILayer : public Layer {
public:
    ImGUILayer();
    ~ImGUILayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnEvent(Event& event);
    virtual void OnEditorRender();

    void Begin();
    void End();

protected:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& event);
    bool OnMouseMovedEvent(MouseMovedEvent& event);
    bool OnMouseScrolledEvent(MouseScrolledEvent& event);

    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnKeyTypedEvent(KeyTypedEvent& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);

    ImGuiKey GetImGuiKeyCode(int keyCode);

private:
    float m_Time{ 0.f };
};
}  // namespace ly
