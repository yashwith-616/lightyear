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

    ApptTick,
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
    None           = 0,
    EC_APPLICATION = BIT(0),
    EC_INPUT       = BIT(1),
    EC_KEYBOARD    = BIT(2),
    EC_MOUSE       = BIT(3),
    EC_MOUSEBUTTON = BIT(4),
    EC_EDITOR      = BIT(5)
};

}  // namespace ly