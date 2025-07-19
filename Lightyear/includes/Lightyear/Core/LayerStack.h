#pragma once

#include "Lightyear/Core/Layer.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

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
class LIGHTYEAR_API LayerStack final {
public:
    LayerStack()                                       = default;
    ~LayerStack()                                      = default;
    LayerStack(const LayerStack&)                      = delete;
    LayerStack& operator=(const LayerStack&)           = delete;
    LayerStack(LayerStack&& other) noexcept            = default;
    LayerStack& operator=(LayerStack&& other) noexcept = default;

    void PushLayer(Scope<Layer> layer);
    void PushOverlay(Scope<Layer> overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    // NOLINTBEGIN
    [[nodiscard]] std::vector<Scope<Layer>>::iterator begin() { return m_Layers.begin(); }
    [[nodiscard]] std::vector<Scope<Layer>>::iterator end() { return m_Layers.end(); }
    [[nodiscard]] std::vector<Scope<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
    [[nodiscard]] std::vector<Scope<Layer>>::const_iterator end() const { return m_Layers.end(); }
    // NOLINTEND

private:
    std::vector<Scope<Layer>> m_Layers;
    int m_LayerInsertIndex{ 0 };
};

}  // namespace ly
