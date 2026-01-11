#pragma once

#include "Event.h"

namespace ly
{

class LIGHTYEAR_API MouseMovedEvent : public EventBase<MouseMovedEvent, EventType::MouseMoved, EC_INPUT | EC_MOUSE>
{
public:
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    float GetX() const { return m_MouseX; }
    float GetY() const { return m_MouseY; }

    std::string ToString() const override { return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY); }

private:
    float m_MouseX{0.f};
    float m_MouseY{0.f};
};

class LIGHTYEAR_API MouseScrolledEvent
    : public EventBase<MouseScrolledEvent, EventType::MouseScrolled, EC_INPUT | EC_MOUSE>
{
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    float GetXOffset() const { return m_XOffset; }
    float GetYOffset() const { return m_YOffset; }

    std::string ToString() const override { return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset); }

private:
    float m_XOffset{0.f};
    float m_YOffset{0.f};
};

// --------------------------- Mouse Button Events ------------------------------------
class LIGHTYEAR_API MouseButtonEvent
{
public:
    int GetMouseButton() const { return m_Button; }

protected:
    MouseButtonEvent(int button) : m_Button(button) {}

    int m_Button;
};

class LIGHTYEAR_API MouseButtonPressedEvent
    : public EventBase<MouseButtonPressedEvent, EventType::MouseButtonPressed, EC_INPUT | EC_MOUSE>,
      public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override { return std::format("MouseButtonPressedEvent: {}", m_Button); }
};

class LIGHTYEAR_API MouseButtonReleasedEvent
    : public EventBase<MouseButtonReleasedEvent, EventType::MouseButtonReleased, EC_INPUT | EC_MOUSE>,
      public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override { return std::format("MouseButtonReleasedEvent: {}", m_Button); }
};

} // namespace ly
