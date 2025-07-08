#pragma once

#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

class LIGHTYEAR_API LayerStack {
public:
    LayerStack() = default;
    virtual ~LayerStack() = default;

    LayerStack(const LayerStack&)            = delete;
    LayerStack& operator=(const LayerStack&) = delete;
    
    LayerStack(const LayerStack&& other) noexcept;
    LayerStack& operator=(LayerStack&& other) noexcept;

    void PushLayer(Scope<Layer> layer);
    void PushOverlay(Scope<Layer> overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Scope<Layer>>::iterator Begin() { return m_Layers.begin(); }
    std::vector<Scope<Layer>>::iterator End() { return m_Layers.end(); }

private:
    // TODO: Use PMR or Arena allocations instead for better performance
    std::vector<Scope<Layer>> m_Layers{};
    size_t m_LayerInsertIndex{ 0 };
};

}  // namespace ly
