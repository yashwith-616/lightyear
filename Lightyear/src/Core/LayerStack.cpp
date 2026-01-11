#include "Lightyear/Core/LayerStack.h"

namespace ly
{

void LayerStack::PushLayer(Scope<Layer> layer)
{
    m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
    ++m_LayerInsertIndex;
}

void LayerStack::PushOverlay(Scope<Layer> overlay) { m_Layers.emplace_back(std::move(overlay)); }

void LayerStack::PopLayer(Layer* layer)
{
    const auto iter = std::find_if(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, [layer](auto& existing) {
        return existing.get() == layer;
    });
    if (iter != m_Layers.begin() + m_LayerInsertIndex)
    {
        m_Layers.erase(iter);
        --m_LayerInsertIndex;
    }
}

void LayerStack::PopOverlay(Layer* overlay)
{
    const auto iter = std::find_if(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), [overlay](auto& existing) {
        return existing.get() == overlay;
    });
    if (iter != m_Layers.end())
    {
        m_Layers.erase(iter);
    }
}

} // namespace ly
