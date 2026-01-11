#pragma once
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene
{

class LIGHTYEAR_API MovementSystem : public ISystem
{
public:
    MovementSystem();

    void Execute(entt::registry& registry) override;

    void Init(entt::registry& registry) override;

    void Shutdown(entt::registry& registry) override;
};

} // namespace ly::scene
