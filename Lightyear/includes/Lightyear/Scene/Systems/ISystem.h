#pragma once

#include <Lightyear/LightyearCore.h>
#include "Lightyear/Core/SystemLayer.h"

LY_DISABLE_WARNINGS_PUSH
#include <entt/entt.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly::scene
{

class LIGHTYEAR_API ISystem
{
public:
    ISystem(std::string name, SystemLayer layer) : m_name(std::move(name)), m_systemLayer(layer) {}
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
    virtual void execute(entt::registry& registry);

    virtual void init(entt::registry& registry) = 0;
    virtual void shutdown(entt::registry& registry) = 0;

    [[nodiscard]] SystemLayer getSystemLayer() const { return m_systemLayer; }
    [[nodiscard]] bool isEnabled() const { return m_isEnabled; }
    [[nodiscard]] std::string getName() const { return m_name; }

    void setEnabled(bool enable) { m_isEnabled = enable; }

private:
    std::string m_name{k_kNotset};
    Uuid m_id{Uuid()};
    bool m_isEnabled{true};
    SystemLayer m_systemLayer{SystemLayer::Gameplay};
};
inline void ISystem::execute(entt::registry& registry) {}

} // namespace ly::scene
