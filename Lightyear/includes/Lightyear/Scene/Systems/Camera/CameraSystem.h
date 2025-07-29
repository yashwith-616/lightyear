#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene {

struct CameraComponent;
struct TransformComponent;

class LIGHTYEAR_API CameraSystem : public ISystem {
public:
    CameraSystem();

    void Init(entt::registry& registry) override;

    void Execute(entt::registry& registry) override;

    void Shutdown(entt::registry& registry) override;

private:
    static void RecalculateViewMatrix(CameraComponent& camera, const TransformComponent& transform);
    static void RecalculateProjectionMatrix(CameraComponent& camera, const TransformComponent& transform);
};

}  // namespace ly::scene