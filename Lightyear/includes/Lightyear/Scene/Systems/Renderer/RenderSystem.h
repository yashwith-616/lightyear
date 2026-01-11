#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene
{

class LIGHTYEAR_API RenderSystem : public ISystem
{
public:
    RenderSystem();

    void Init(entt::registry& registry) override;

    void Execute(entt::registry& registry) override;

    void Shutdown(entt::registry& registry) override;
};

} // namespace ly::scene
