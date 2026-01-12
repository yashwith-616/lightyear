#pragma once

#include <Lightyear/LightyearCore.h>
#include "Lightyear/Core/LayerStack.h"
#include "Lightyear/Core/Window.h"

namespace ly {

class Event;
class WindowCloseEvent;

/**
 * @brief Represents the core application entry point and lifecycle manager for the Lightyear Engine.
 *
 * @details
 * The `Application` class serves as central hub for any lightyear Engine based program. It manages the main application
 * loop, handles window creation, process events and maintains the layered architecture for modular functionality
 * (Layers and Overlays)
 *
 * @note Designed as a **singleton** only one instance of `Application` can exist globally. **Clients must implement an
 * external factory function `CreateApplication()`** that returns a `Scope<Application>` to their derived application
 * class. The framework's internal `main()` function will then call this factory and pass the returned instance to
 * `Application::Create()`.
 *
 * @section ThreadSafety
 * This class is not inherently thread-safe by default. All interactions with the `Application` instance should
 * typically occur on the main thread unless explicitly stated otherwise for specific methods.
 */
class LIGHTYEAR_API Application {
public:
    virtual ~Application();

    Application(Application const&)            = delete;
    Application& operator=(Application const&) = delete;
    Application(Application&&)                 = delete;
    Application& operator=(Application&&)      = delete;

    /**
     * The following factory method is used for initializing the Application
     *
     * @param app the application ownership
     */
    static void create(scope<Application> app);

    /**
     * Get Reference to Application that has been initialized
     *
     * @return the singleton Application
     */
    static Application& get();

    /**
     * @brief Initializes the application's core subsystems.
     *
     * This method is called once during application startup, immediately after the application
     * instance has been created and before the main game loop begins.
     * Override this to perform custom, one-time application initialization.
     */
    virtual void init();

    /**
     * @brief Starts and manages the main application loop.
     * This method enters the core application loop, which continuously handles
     * events, updates the application state, and renders frames until the
     * application is requested to shut down (e.g., window closed).
     * Override this to implement a custom application loop.
     */
    virtual void run();

    /**
     * @brief Handles incoming application events.
     *
     * This virtual method allows the application to handle events globally before they are passed
     * down to the layer stack. Derived classes can override this to implement custom event handling
     * logic and can potentially mark the event as handled to stop further propagation.
     *
     * @param event The event that occurred. Derived classes can modify this event.
     */
    virtual void onEvent(Event& event);

    /**
     * Adds a layer to the application's layer stack.
     *
     * @param layer The layer to add. Ownership is transferred.
     */
    virtual void pushLayer(scope<Layer> layer);

    /**
     * Adds an overlay to the application's layer stack.
     *
     * @param overlay The overlay to add. Ownership is transferred.
     */
    virtual void pushOverlay(scope<Layer> overlay);

    [[nodiscard]] Window& getWindow() { return *m_window; }
    [[nodiscard]] Window const& getWindow() const { return *m_window; }

protected:
    Application();

    virtual bool onWindowClose(WindowCloseEvent& event);

private:
    static scope<Application> m_instance;

    scope<Window> m_window;
    LayerStack m_layerStack;

    bool m_isRunning{ true };
    float m_frametime{ k_kDefaultFrametime };
    float m_lastFrameTime{ 0.f };
};

}  // namespace ly
