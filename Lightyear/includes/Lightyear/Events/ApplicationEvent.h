#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API WindowResizeEvent : public Event {
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

    inline unsigned int GetWidth() { return m_Width; }
    inline unsigned int GetHeight() { return m_Height; }

    std::string ToString() const override {
        return std::format("WindowResizeEvent: {}, {}", m_Width, m_Height);
    }

    EVENT_CLASS_CATEGORY(EC_APPLICATION)
    EVENT_CLASS_TYPE(WindowResize)

private:
    unsigned int m_Width;
    unsigned int m_Height;
};

class LIGHTYEAR_API WindowCloseEvent : public Event {
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EC_APPLICATION)
};

// TODO: The below may not be required
class LIGHTYEAR_API AppTickEvent : public Event {};

class LIGHTYEAR_API AppUpdatedEvent : public Event {};

class LIGHTYEAR_API AppRenderEvent : public Event {};
}  // namespace ly
