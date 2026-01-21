#pragma once

#include <Lightyear/LightyearCore.h>
#include "Lightyear/Core/Layer.h"

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

class LIGHTYEAR_API ImGuiLayer : public Layer
{
public:
    ImGuiLayer() : Layer("ImGUILayer") {}
    ~ImGuiLayer() override;

    /**
     * @brief Initialize ImGUI backends such as Window manager and the RHI
     */
    static void beginFrame();

    /**
     * @brief Prepare ImGUI for next update. Render the current update.
     */
    static void endFrame();

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event& event) override;
    void onEditorRender() override;
    void onUpdate(float deltaTime) override {}

protected:
    bool onMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool onMouseButtonReleaseEvent(MouseButtonReleasedEvent& event);
    bool onMouseMovedEvent(MouseMovedEvent& event);
    bool onMouseScrolledEvent(MouseScrolledEvent& event);

    bool onKeyPressedEvent(KeyPressedEvent& event);
    bool onKeyReleasedEvent(KeyReleasedEvent& event);
    bool onKeyTypedEvent(KeyTypedEvent& event);
    bool onWindowResizeEvent(WindowResizeEvent& event);

    static ImGuiKey getImGuiKeyCode(int keyCode);
};
} // namespace ly
