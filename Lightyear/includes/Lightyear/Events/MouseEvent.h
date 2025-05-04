#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    inline float GetX() const { return m_MouseX; }
    inline float GetY() const { return m_MouseY; }

    CText ToString() const override {
        return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EC_INPUT | EC_MOUSE)

private:
    float m_MouseX{ 0.f };
    float m_MouseY{ 0.f };
};

class LIGHTYEAR_API MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    inline float GetXOffset() const { return m_XOffset; }
    inline float GetYOffset() const { return m_YOffset; }

    CText ToString() const override {
        return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EC_INPUT | EC_MOUSE)

private:
    float m_XOffset{ 0.f };
    float m_YOffset{ 0.f };
};

class LIGHTYEAR_API MouseButtonEvent : public Event {
public:
    inline int GetMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EC_INPUT | EC_MOUSE)
protected:
    MouseButtonEvent(int button) : m_Button(button) {}

    int m_Button;
};

class LIGHTYEAR_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    CText ToString() const override { return std::format("MouseButtonPressedEvent: {}", m_Button); }
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class LIGHTYEAR_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    CText ToString() const override {
        return std::format("MouseButtonReleasedEvent: {}", m_Button);
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}  // namespace ly
