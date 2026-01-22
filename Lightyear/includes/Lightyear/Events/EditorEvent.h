#pragma once

#include "Event.h"

namespace ly
{

class LIGHTYEAR_API EditorUpdateBeginEvent
    : public EventBase<EditorUpdateBeginEvent, EventType::EditorUpdateBegin, EcEditor>
{
public:
    EditorUpdateBeginEvent() = default;

    std::string toString() const override { return std::format("EditorUpdateBeginEvent"); }
};

class LIGHTYEAR_API EditorUpdateEndEvent
    : public EventBase<EditorUpdateEndEvent, EventType::EditorUpdateBegin, EcEditor>
{
public:
    EditorUpdateEndEvent() = default;

    std::string toString() const override { return std::format("EditorUpdateEndEvent"); }
};

} // namespace ly
