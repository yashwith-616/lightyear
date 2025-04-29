#pragma once

#include <lypch.h>
#include "LightYear/LightyearCore.h"

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
    MouseScrolled
};

enum EventCategory {
    None           = 0,
    EC_APPLICATION = BIT(0),
    EC_INPUT       = BIT(1),
    EC_KEYBOARD    = BIT(2),
    EC_MOUSE       = BIT(3),
    EC_MOUSEBUTTON = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType() {                                                                                 \
        return EventType::##type;                                                                                      \
    }                                                                                                                  \
    virtual EventType GetEventType() const override {                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual std::string_view GetName() const override {                                                                \
        return #type;                                                                                                  \
    }

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual int GetCategoryFlags() const override {                                                                    \
        return category;                                                                                               \
    }

/**
 * @brief The lightyear Event class
 */
class LIGHTYEAR_API Event {
    friend class EventDispatcher;

public:
    virtual ~Event() = default;
    bool bIsHandled{ false };

    virtual EventType GetEventType() const   = 0;
    virtual std::string_view GetName() const = 0;
    virtual int GetCategoryFlags() const     = 0;
    virtual std::string ToString() const { return static_cast<std::string>(GetName()); }

    inline bool IsInCategory(EventCategory category) const noexcept { return GetCategoryFlags() & category; }

protected:
    bool m_Handled{ false };
};

/**
 * @brief Event dispatcher
 */
class EventDispatcher {
public:
    EventDispatcher(Event& event) : m_Event(event) {}

    template <typename T, typename F>
    bool Dispatch(F&& func) {
        static_assert(std::is_base_of<Event, T>::value, "T must inherit from Event");

        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.m_Handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

}  // namespace ly
