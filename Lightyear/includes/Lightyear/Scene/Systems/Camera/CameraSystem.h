#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Systems/ISystem.h"

namespace ly::scene
{

struct CameraComponent;
struct TransformComponent;

class LIGHTYEAR_API CameraSystem : public ISystem
{
public:
    CameraSystem();

    void init(entt::registry& registry) override;

    void execute(entt::registry& registry) override;

    void shutdown(entt::registry& registry) override;

private:
    static void recalculateViewMatrix(CameraComponent& camera, TransformComponent const& transform);
    static void recalculateProjectionMatrix(CameraComponent& camera);
};

} // namespace ly::scene
