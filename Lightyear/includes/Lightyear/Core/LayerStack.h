#pragma once

#include <lypch.h>
#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API LayerStack {
public:
    LayerStack();
    ~LayerStack() = default;

    LayerStack(const LayerStack&)            = delete;
    LayerStack& operator=(const LayerStack&) = delete;

    void PushLayer(Scope<Layer> layer);
    void PushOverlay(Scope<Layer> overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Scope<Layer>>::iterator begin() { return m_Layers.begin(); }
    std::vector<Scope<Layer>>::iterator end() { return m_Layers.end(); }

private:
    std::vector<Scope<Layer>> m_Layers;
    std::vector<Scope<Layer>>::iterator m_LayerInsert;
};

}  // namespace ly
