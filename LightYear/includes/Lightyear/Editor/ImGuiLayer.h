#pragma once

#include "LightYear/Core/Layer.h"
#include "Lightyear/LightyearAPI.h"

namespace ly {

class LIGHTYEAR_API ImGuiLayer : public Layer {
public:
    ImGuiLayer() : Layer("ImguiLayer") {}
    ~ImGuiLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnEvent(Event& event);
};
}  // namespace ly
