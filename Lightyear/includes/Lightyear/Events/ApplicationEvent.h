#pragma once

#include "Event.h"

namespace ly
{

class LIGHTYEAR_API WindowResizeEvent : public EventBase<WindowResizeEvent, EventType::WindowResize, EcApplication>
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    [[nodiscard]] uint32_t getWidth() const { return m_width; }
    [[nodiscard]] uint32_t getHeight() const { return m_height; }

    std::string toString() const override { return std::format("WindowResizeEvent: {}, {}", m_width, m_height); }

private:
    unsigned int m_width{};
    unsigned int m_height{};
};

class LIGHTYEAR_API WindowCloseEvent : public EventBase<WindowCloseEvent, EventType::WindowClose, EcApplication>
{
public:
    WindowCloseEvent() = default;
};

} // namespace ly
