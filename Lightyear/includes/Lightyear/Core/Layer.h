#pragma once

#include "Lightyear/Events/Event.h"
#include "Lightyear/LightyearCore.h"

namespace ly {

/**
 * @brief Base class for all application layers and overlays.
 *
 * @details
 * The `Layer` class provides a fundamental interface for modular application components
 * that can be pushed onto a `LayerStack`. Layers are designed to handle various aspects
 * of the application's lifecycle, including updates, rendering, and event processing.
 *
 * Derived classes should inherit from `Layer` and override its virtual methods
 * to implement custom behavior specific to their role (e.g., game scene, UI element, debug overlay).
 */
class LIGHTYEAR_API Layer {
public:
    /**
     * @brief Constructs a Layer with an optional name and assigns a unique ID.
     *
     * @param name The human-readable name of the layer, useful for debugging and identification.
     * Defaults to "None" if not provided.
     */
    explicit Layer(std::string name) : m_name(std::move(name)), m_id(Uuid()) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived Layer types.
     *
     * @details When a Layer is removed from the LayerStack, its destructor (and thus
     * the destructors of any derived classes) will be called automatically.
     */
    virtual ~Layer() = default;

    Layer(const Layer&)                      = delete;
    Layer& operator=(const Layer&)           = delete;
    Layer(Layer&& other) noexcept            = default;
    Layer& operator=(Layer&& other) noexcept = default;

    /**
     * @brief Called when the layer is attached to a LayerStack.
     *
     * @details Use this method for one-time initialization tasks that depend on the layer
     * being active in the application's context (e.g., setting up rendering resources,
     * registering callbacks, loading assets).
     */
    virtual void onAttach() = 0;

    /**
     * @brief Called when the layer is detached from a LayerStack.
     *
     * @details Use this method for cleanup tasks when the layer is no longer active
     * (e.g., releasing resources, unregistering callbacks, saving state).
     */
    virtual void onDetach() = 0;

    /**
     * @brief Called every frame to update the layer's state.
     *
     * @details This is the main update loop for the layer's logic. Derived classes should
     * implement game logic, animations, physics updates, etc., here.
     *
     * @param deltaTime The time elapsed since the last frame, in seconds.
     */
    virtual void onUpdate(float deltaTime) = 0;

    /**
     * @brief Called every frame for editor-specific rendering.
     *
     * @details This method is intended for rendering debug gizmos, editor UI elements,
     * or other visuals that should only appear in an editor context and not
     * in a standalone runtime build.
     */
    virtual void onEditorRender() = 0;

    /**
     * @brief Called when an event is dispatched to this layer.
     *
     * @details Layers typically process events by checking the event type and handling it accordingly.
     * Events are usually dispatched from the `Application` through the `LayerStack`.
     *
     * @param event The event to be processed by the layer.
     */
    virtual void onEvent(Event& event) = 0;

    [[nodiscard]] std::string const& getName() const { return m_name; }
    [[nodiscard]] Uuid getId() const { return m_id; }

private:
    std::string m_name{ "None" };
    Uuid m_id;
};

}  // namespace ly