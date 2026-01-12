#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

enum class EventType : uint8_t {
    None = 0,

    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,

    AppTick,
    AppUpdate,
    AppRender,

    KeyPressed,
    KeyReleased,
    KeyTyped,

    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,

    EditorUpdateBegin,
    EditorUpdateEnd,
};

enum EventCategory {
    EcApplication = bit(0),
    EcInput       = bit(1),
    EcKeyboard    = bit(2),
    EcMouse       = bit(3),
    EcMousebutton = bit(4),
    EcEditor      = bit(5)
};

}  // namespace ly