#pragma once

#include "EventTypes.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

/**
 * @brief The lightyear Event class
 */
class LIGHTYEAR_API Event {
    friend class EventDispatcher;

public:
    virtual ~Event() = default;

    virtual EventType GetEventType() const   = 0;
    virtual std::string_view GetName() const = 0;
    virtual int GetCategoryFlags() const     = 0;
    virtual std::string ToString() const { return std::format("String: {}", GetName()); }

    inline bool IsInCategory(EventCategory category) const noexcept {
        return GetCategoryFlags() & category;
    }

    inline bool IsHandled() const { return m_Handled; }
    inline void SetHandled(bool handled) noexcept { m_Handled = handled; }

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

        if (m_Event.GetEventType() == T::StaticType) {
            m_Event.m_Handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

/**
 * @brief Event Base class to be used in CRTP template. This allows to avoid boilerplate
 * implementation of some common types
 *
 * @tparam Derived the Derived class name
 * @tparam Type the EventType
 * @tparam CategoryFlags the category the event falls under
 */
template <typename Derived, EventType Type, int CategoryFlags>
class EventBase : public Event {
public:
    static constexpr EventType StaticType = Type;

    EventType GetEventType() const override { return StaticType; }

    std::string_view GetName() const override { return typeid(Derived).name(); }

    int GetCategoryFlags() const override { return CategoryFlags; }
};

}  // namespace ly
