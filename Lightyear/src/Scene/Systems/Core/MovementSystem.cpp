#include "Lightyear/Scene/Systems/Core/MovementSystem.h"

namespace ly::scene {

MovementSystem::MovementSystem() : ISystem("MovementSystemV1", SystemLayer::Gameplay) {}

void MovementSystem::Init(entt::registry& registry) {}

void MovementSystem::Shutdown(entt::registry& registry) {}

void MovementSystem::Execute(entt::registry& registry) {}

}  // namespace ly::scene