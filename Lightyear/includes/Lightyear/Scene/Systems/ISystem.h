#pragma once

#include "Lightyear/Core/SystemLayer.h"
#include "Lightyear/LightyearCore.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene {

class LIGHTYEAR_API ISystem {
public:
    ISystem(std::string name, SystemLayer layer) : m_Name(std::move(name)), m_SystemLayer(layer) {}
    virtual ~ISystem() = default;

    /**
     * All systems in lightyear should extend from this base class and register themselves under a systemRegistry.
     * System registry groups systems based on layers where it needs to be executed. It also keeps track of the scene.
     *
     * Every unique scene will have a custom registry loaded. This allows editor world such as Animation/Material
     * editors to load/run lesser number of systems saving performance in the editor.
     *
     * TODO: Pass the World instead. Require to abstract Scene inside a World. World object will be passed around.
     */
    virtual void Execute(entt::registry& registry) = 0;

    virtual void Init(entt::registry& registry)     = 0;
    virtual void Shutdown(entt::registry& registry) = 0;

    [[nodiscard]] SystemLayer GetSystemLayer() const { return m_SystemLayer; }
    [[nodiscard]] bool IsEnabled() const { return m_IsEnabled; }

    void SetEnabled(bool enable) { m_IsEnabled = enable; }

private:
    std::string m_Name{ kNOTSET };
    UUID m_ID{ UUID() };
    bool m_IsEnabled{ true };
    SystemLayer m_SystemLayer{ SystemLayer::Gameplay };
};

}  // namespace ly::scene