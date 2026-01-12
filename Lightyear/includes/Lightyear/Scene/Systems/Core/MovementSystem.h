#pragma once
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene {

class LIGHTYEAR_API MovementSystem : public ISystem {
public:
    MovementSystem();

    void execute(entt::registry& registry) override;

    void init(entt::registry& registry) override;

    void shutdown(entt::registry& registry) override;
};

}  // namespace ly::scene