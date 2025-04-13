#pragma once

#include <lypch.h>

namespace Lightyear::Events {

constexpr uint8_t BIT(uint8_t x) { return 1 << x; }

enum class EventType : uint8_t {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory {
    None = 0,
    EC_APPLICATION = BIT(0),
    EC_INPUT = BIT(1),
    EC_KEYBOARD = BIT(2),
    EC_MOUSE = BIT(3),
    EC_MOUSEBUTTON = BIT(4)
};

class Event {
public:
    virtual ~Event() = default;
    bool bIsHandled{ false; }

    virtual EventType GetEventType() const = 0;
    virtual std::string_view GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) const noexcept {
        return GetCategoryFlags() & category;
    }
};

}
