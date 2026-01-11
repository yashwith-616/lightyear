#pragma once

#include "Event.h"
#include "EventTypes.h"

namespace ly
{

class LIGHTYEAR_API KeyEvent
{
public:
    int GetKeyCode() const { return m_KeyCode; }

protected:
    explicit KeyEvent(int keyCode) : m_KeyCode(keyCode) {}

    int m_KeyCode;
};

class LIGHTYEAR_API KeyPressedEvent : public EventBase<KeyPressedEvent, EventType::KeyPressed, EC_KEYBOARD | EC_INPUT>,
                                      public KeyEvent
{
public:
    KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        return std::format("KeyPressedEvent: {} ({} repeats)", m_KeyCode, m_RepeatCount);
    }

protected:
    int m_RepeatCount{0};
};

class LIGHTYEAR_API KeyReleasedEvent
    : public EventBase<KeyReleasedEvent, EventType::KeyReleased, EC_KEYBOARD | EC_INPUT>,
      public KeyEvent
{
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override { return std::format("KeyReleasedEvent: {}", m_KeyCode); }
};

class LIGHTYEAR_API KeyTypedEvent : public EventBase<KeyTypedEvent, EventType::KeyTyped, EC_KEYBOARD | EC_INPUT>,
                                    public KeyEvent
{
public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override { return std::format("KeyTypedEvent: {}", m_KeyCode); }
};

} // namespace ly
