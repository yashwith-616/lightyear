#pragma once

#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly
{

/**
 * @brief Manages the ordered collection of active Layers and Overlays in the application's runtime.
 *
 * @details
 * The `LayerStack` is a core component within the Lightyear Engine, primarily owned and managed by the
 * `ly::Application` instance. Its fundamental role is to maintain an ordered collection of `ly::Layer` objects,
 * ensuring a precise processing sequence during the application's main loop.
 *
 * @section LayerTypes Layer Categorization
 * The `LayerStack` conceptually segregates layers into two distinct categories, both derived from `ly::Layer`:
 * -   **Regular Layers:** These are typically core game logic, scene managers, game states, or application modules.
 * They are pushed onto the stack via `PushLayer()` and occupy the lower part of the stack.
 * -   **Overlays:** These are generally user interface (UI) elements, debug tools, or other components that should
 * always be processed and rendered on top of all regular layers. They are pushed via `PushOverlay()`
 * and reside at the higher end of the stack.
 */
class LIGHTYEAR_API LayerStack final
{
public:
    LayerStack() = default;
    ~LayerStack() = default;
    LayerStack(LayerStack const&) = delete;
    LayerStack& operator=(LayerStack const&) = delete;
    LayerStack(LayerStack&& other) noexcept = default;
    LayerStack& operator=(LayerStack&& other) noexcept = default;

    void pushLayer(scope<Layer> layer);
    void pushOverlay(scope<Layer> overlay);
    void popLayer(Layer* layer);
    void popOverlay(Layer* overlay);

    // NOLINTBEGIN
    [[nodiscard]] std::vector<scope<Layer>>::iterator begin() { return m_layers.begin(); }
    [[nodiscard]] std::vector<scope<Layer>>::iterator end() { return m_layers.end(); }
    [[nodiscard]] std::vector<scope<Layer>>::const_iterator begin() const { return m_layers.begin(); }
    [[nodiscard]] std::vector<scope<Layer>>::const_iterator end() const { return m_layers.end(); }
    // NOLINTEND

private:
    std::vector<scope<Layer>> m_layers;
    int m_layerInsertIndex{0};
};

} // namespace ly
