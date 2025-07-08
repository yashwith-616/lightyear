#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly {

class Event;

class LIGHTYEAR_API Layer {
public:
    Layer(std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(float deltaTime);
    virtual void OnEditorRender();
    virtual void OnEvent(Event& event);

    inline const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

}  // namespace ly