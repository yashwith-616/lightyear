#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API EditorUpdateBeginEvent
    : public EventBase<EditorUpdateBeginEvent, EventType::EditorUpdateBegin, EC_EDITOR> {
public:
    EditorUpdateBeginEvent() {}

    std::string ToString() const override { return std::format("EditorUpdateBeginEvent"); }
};

class LIGHTYEAR_API EditorUpdateEndEvent
    : public EventBase<EditorUpdateEndEvent, EventType::EditorUpdateBegin, EC_EDITOR> {
public:
    EditorUpdateEndEvent() {}

    std::string ToString() const override { return std::format("EditorUpdateEndEvent"); }
};

}  // namespace ly
