#pragma once

#include "Event.h"

namespace ly {

class LIGHTYEAR_API EditorUpdateBeginEvent : public Event {
public:
    EditorUpdateBeginEvent() {}

    CText ToString() const override { return std::format("EditorUpdateBeginEvent"); }

    EVENT_CLASS_CATEGORY(EC_EDITOR)
    EVENT_CLASS_TYPE(EditorUpdateBegin)
};

class LIGHTYEAR_API EditorUpdateEndEvent : public Event {
public:
    EditorUpdateEndEvent() {}

    CText ToString() const override { return std::format("EditorUpdateEndEvent"); }

    EVENT_CLASS_CATEGORY(EC_EDITOR)
    EVENT_CLASS_TYPE(EditorUpdateEnd)
};

}  // namespace ly
