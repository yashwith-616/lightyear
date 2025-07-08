#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API WindowResizeEvent
    : public EventBase<WindowResizeEvent, EventType::WindowResize, EC_APPLICATION> {
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

    inline uint32_t GetWidth() { return m_Width; }
    inline uint32_t GetHeight() { return m_Height; }

    std::string ToString() const override {
        return std::format("WindowResizeEvent: {}, {}", m_Width, m_Height);
    }

private:
    unsigned int m_Width{};
    unsigned int m_Height{};
};

class LIGHTYEAR_API WindowCloseEvent
    : public EventBase<WindowCloseEvent, EventType::WindowClose, EC_APPLICATION> {
public:
    WindowCloseEvent() {}
};

}  // namespace ly
