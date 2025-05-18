#pragma once

#include <lypch.h>
#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

class LIGHTYEAR_API Layer {
public:
    Layer(const CName& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(float deltaTime);
    virtual void OnEditorRender();
    virtual void OnEvent(Event& event);

    inline const CName& GetName() const { return m_DebugName; }

protected:
    CName m_DebugName;
};

}  // namespace ly