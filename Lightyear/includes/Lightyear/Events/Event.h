#pragma once

#include "EventTypes.h"

namespace ly {

/**
 * @brief The lightyear Event class
 */
class LIGHTYEAR_API Event {
    friend class EventDispatcher;

public:
    virtual ~Event() = default;

    virtual EventType getEventType() const   = 0;
    virtual std::string_view getName() const = 0;
    virtual int getCategoryFlags() const     = 0;
    virtual std::string toString() const { return std::format("String: {}", getName()); }

    bool isInCategory(EventCategory category) const noexcept { return getCategoryFlags() & category; }

    bool isHandled() const { return m_handled; }
    void setHandled(bool isHandled) noexcept { m_handled = isHandled; }

protected:
    bool m_handled{ false };
};

/**
 * @brief Event dispatcher
 */
class EventDispatcher {
public:
    EventDispatcher(Event& event) : m_event(event) {}

    template <typename T, typename F>
    bool dispatch(F&& func) {
        static_assert(std::is_base_of_v<Event, T>, "T must inherit from Event");

        if (m_event.getEventType() == T::StaticType) {
            m_event.m_handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }

private:
    Event& m_event;
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
    static constexpr EventType k_staticType = Type;

    EventType getEventType() const override { return k_staticType; }

    std::string_view getName() const override { return typeid(Derived).name(); }

    int getCategoryFlags() const override { return CategoryFlags; }
};

}  // namespace ly
