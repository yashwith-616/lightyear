#include "Lightyear/Core/LayerStack.h"

namespace ly {

LayerStack::LayerStack() {
    m_LayerInsert = m_Layers.begin();
}

void LayerStack::PushLayer(Scope<Layer> layer) {
    m_LayerInsert = m_Layers.emplace(m_LayerInsert, std::move(layer));
}

void LayerStack::PushOverlay(Scope<Layer> overlay) {
    m_Layers.emplace_back(std::move(overlay));
}

void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [layer](const Scope<Layer>& l) {
        return l.get() == layer;
    });
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
        m_LayerInsert--;
    }
}

void LayerStack::PopOverlay(Layer* overlay) {
    auto it = std::find_if(m_Layers.begin(), m_Layers.end(), [overlay](const Scope<Layer>& l) {
        return l.get() == overlay;
    });
    if (it != m_Layers.end()) {
        m_Layers.erase(it);
        m_LayerInsert--;
    }
}

}  // namespace ly