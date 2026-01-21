#include "Lightyear/Scene/Systems/Core/MovementSystem.h"

namespace ly::scene
{

MovementSystem::MovementSystem() : ISystem("MovementSystemV1", SystemLayer::Gameplay) {}

void MovementSystem::execute(entt::registry& registry) {}

void MovementSystem::init(entt::registry& registry) {}

void MovementSystem::shutdown(entt::registry& registry) {}

} // namespace ly::scene
