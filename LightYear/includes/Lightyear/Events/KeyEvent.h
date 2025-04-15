#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API KeyEvent : public Event {
public:
    inline int GetKeyCode() const { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EC_KEYBOARD | EC_INPUT)

protected:
    explicit KeyEvent(int keyCode)
        : m_KeyCode(keyCode)
    {}

    int m_KeyCode;
};

class LIGHTYEAR_API KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode)
        , m_RepeatCount(repeatCount)
    {}

    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        return std::format("KeyPressedEvent: {} ({} repeats)", m_KeyCode, m_RepeatCount);
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_RepeatCount { 0 };
};


class LIGHTYEAR_API KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override {
        return std::format("KeyReleasedEvent: {}", m_KeyCode);
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

}
