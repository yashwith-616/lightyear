#include "Lightyear/Scene/Core/Entity.h"

namespace ly::scene
{

Entity::Entity(entt::entity handle, Scene* scene) : m_entityHandle(handle), m_scene(scene) {}

} // namespace ly::scene
