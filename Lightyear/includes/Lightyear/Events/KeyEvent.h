#pragma once

#include "Event.h"
#include "EventTypes.h"

namespace ly {

class LIGHTYEAR_API KeyEvent {
public:
    int getKeyCode() const { return m_keyCode; }

protected:
    explicit KeyEvent(int keyCode) : m_keyCode(keyCode) {}

    int m_keyCode;
};

class LIGHTYEAR_API KeyPressedEvent : public EventBase<KeyPressedEvent, EventType::KeyPressed, EcKeyboard | EcInput>,
                                      public KeyEvent {
public:
    KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

    int getRepeatCount() const { return m_repeatCount; }

    std::string toString() const override {
        return std::format("KeyPressedEvent: {} ({} repeats)", m_keyCode, m_repeatCount);
    }

protected:
    int m_repeatCount{ 0 };
};

class LIGHTYEAR_API KeyReleasedEvent : public EventBase<KeyReleasedEvent, EventType::KeyReleased, EcKeyboard | EcInput>,
                                       public KeyEvent {
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string toString() const override { return std::format("KeyReleasedEvent: {}", m_keyCode); }
};

class LIGHTYEAR_API KeyTypedEvent : public EventBase<KeyTypedEvent, EventType::KeyTyped, EcKeyboard | EcInput>,
                                    public KeyEvent {
public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    std::string toString() const override { return std::format("KeyTypedEvent: {}", m_keyCode); }
};

}  // namespace ly
