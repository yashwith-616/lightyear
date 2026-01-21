#pragma once

#include "Event.h"

namespace ly
{

class LIGHTYEAR_API MouseMovedEvent : public EventBase<MouseMovedEvent, EventType::MouseMoved, EcInput | EcMouse>
{
public:
    MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

    float getX() const { return m_mouseX; }
    float getY() const { return m_mouseY; }

    std::string toString() const override { return std::format("MouseMovedEvent: {}, {}", m_mouseX, m_mouseY); }

private:
    float m_mouseX{0.f};
    float m_mouseY{0.f};
};

class LIGHTYEAR_API MouseScrolledEvent
    : public EventBase<MouseScrolledEvent, EventType::MouseScrolled, EcInput | EcMouse>
{
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

    float getXOffset() const { return m_xOffset; }
    float getYOffset() const { return m_yOffset; }

    std::string toString() const override { return std::format("MouseScrolledEvent: {}, {}", m_xOffset, m_yOffset); }

private:
    float m_xOffset{0.f};
    float m_yOffset{0.f};
};

// --------------------------- Mouse Button Events ------------------------------------
class LIGHTYEAR_API MouseButtonEvent
{
public:
    int getMouseButton() const { return m_button; }

protected:
    MouseButtonEvent(int button) : m_button(button) {}

    int m_button;
};

class LIGHTYEAR_API MouseButtonPressedEvent
    : public EventBase<MouseButtonPressedEvent, EventType::MouseButtonPressed, EcInput | EcMouse>,
      public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string toString() const override { return std::format("MouseButtonPressedEvent: {}", m_button); }
};

class LIGHTYEAR_API MouseButtonReleasedEvent
    : public EventBase<MouseButtonReleasedEvent, EventType::MouseButtonReleased, EcInput | EcMouse>,
      public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string toString() const override { return std::format("MouseButtonReleasedEvent: {}", m_button); }
};

} // namespace ly
