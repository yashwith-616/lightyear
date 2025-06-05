#include "Lightyear/Scene/SceneManager.h"

namespace ly {

SceneManager& SceneManager::Get() {
    // TODO: insert return statement here
    return *(new SceneManager());
}

entt::entity SceneManager::CreateEntity(const std::string& name) {
    // TODO: insert return statement here
    return entt::null;
}

void SceneManager::Init() {}

void SceneManager::DestoryEntity(entt::entity& entity) {}

}  // namespace ly