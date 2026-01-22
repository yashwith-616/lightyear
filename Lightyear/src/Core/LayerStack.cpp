#include "Lightyear/Core/LayerStack.h"

namespace ly
{

void LayerStack::pushLayer(scope<Layer> layer)
{
    m_layers.insert(m_layers.begin() + m_layerInsertIndex, std::move(layer));
    ++m_layerInsertIndex;
}

void LayerStack::pushOverlay(scope<Layer> overlay) { m_layers.emplace_back(std::move(overlay)); }

void LayerStack::popLayer(Layer* layer)
{
    auto const iter = std::find_if(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, [layer](auto& existing) {
        return existing.get() == layer;
    });
    if (iter != m_layers.begin() + m_layerInsertIndex)
    {
        m_layers.erase(iter);
        --m_layerInsertIndex;
    }
}

void LayerStack::popOverlay(Layer* overlay)
{
    auto const iter = std::find_if(m_layers.begin() + m_layerInsertIndex, m_layers.end(), [overlay](auto& existing) {
        return existing.get() == overlay;
    });
    if (iter != m_layers.end())
    {
        m_layers.erase(iter);
    }
}

} // namespace ly
