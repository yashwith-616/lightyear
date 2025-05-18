#pragma once

#include <lypch.h>
#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API LayerStack {
public:
    LayerStack() {};
    ~LayerStack();

    LayerStack(const LayerStack&)            = delete;
    LayerStack& operator=(const LayerStack&) = delete;

    void PushLayer(Scope<Layer> layer);
    void PushOverlay(Scope<Layer> overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Scope<Layer>>::iterator begin() { return m_Layers.begin(); }
    std::vector<Scope<Layer>>::iterator end() { return m_Layers.end(); }

private:
    // TODO: Use PMR or Arena allocations instead for better performance
    std::vector<Scope<Layer>> m_Layers{};
    size_t m_LayerInsertIndex{ 0 };
};

}  // namespace ly
