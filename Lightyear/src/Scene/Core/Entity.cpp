#include "Lightyear/Scene/Core/Entity.h"

namespace ly::scene {

Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) {}

}  // namespace ly::scene
