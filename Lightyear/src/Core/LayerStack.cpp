#include "Lightyear/Core/LayerStack.h"

namespace ly {

LayerStack::LayerStack(const LayerStack&& other) noexcept
    : m_Layers(std::move(other.m_Layers)), m_LayerInsertIndex(other.m_LayerInsertIndex) {}

LayerStack& LayerStack::operator=(LayerStack&& other) noexcept {
    if (this != &other) {
        m_Layers = std::move(other.m_Layers);
        m_LayerInsertIndex = other.m_LayerInsertIndex;

        other.m_LayerInsertIndex = 0;
    }
    return *this;
}

void LayerStack::PushLayer(Scope<Layer> layer) {
    m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
    ++m_LayerInsertIndex;
}

void LayerStack::PushOverlay(Scope<Layer> overlay) {
    m_Layers.emplace_back(std::move(overlay));
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find_if(m_Layers.begin(),
                           m_Layers.begin() + m_LayerInsertIndex,
                           [layer](auto& l) { return l.get() == layer; });
    if (it != m_Layers.begin() + m_LayerInsertIndex) {
        m_Layers.erase(it);
        --m_LayerInsertIndex;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find_if(m_Layers.begin() + m_LayerInsertIndex,
                           m_Layers.end(),
                           [overlay](auto& l) { return l.get() == overlay; });
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
    }
}

}  // namespace ly