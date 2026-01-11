#pragma once

#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

enum ImGuiKey : int;

namespace ly
{

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

class LIGHTYEAR_API ImGUILayer : public Layer
{
public:
    ImGUILayer() : Layer("ImGUILayer") {}
    ~ImGUILayer() override;

    /**
     * @brief Initialize ImGUI backends such as Window manager and the RHI
     */
    static void BeginFrame();

    /**
     * @brief Prepare ImGUI for next update. Render the current update.
     */
    static void EndFrame();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Event& event) override;
    void OnEditorRender() override;
    void OnUpdate(float deltaTime) override {}

protected:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleaseEvent(MouseButtonReleasedEvent& event);
    bool OnMouseMovedEvent(MouseMovedEvent& event);
    bool OnMouseScrolledEvent(MouseScrolledEvent& event);

    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnKeyTypedEvent(KeyTypedEvent& event);
    bool OnWindowResizeEvent(WindowResizeEvent& event);

    static ImGuiKey GetImGuiKeyCode(int keyCode);
};
} // namespace ly
