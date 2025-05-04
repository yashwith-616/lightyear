#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API WindowResizeEvent : public Event {
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

    inline uint32_t GetWidth() { return m_Width; }
    inline uint32_t GetHeight() { return m_Height; }

    CText ToString() const override {
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

}  // namespace ly
