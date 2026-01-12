#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene {

class LIGHTYEAR_API RenderSystem : public ISystem {
public:
    RenderSystem();

    void init(entt::registry& registry) override;

    void execute(entt::registry& registry) override;

    void shutdown(entt::registry& registry) override;
};

}  // namespace ly::scene