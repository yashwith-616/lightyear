#include "Lightyear/World/Level.h"

#include "Lightyear/Scene/Components/Core/DirtyComponent.h"

#include "Lightyear/Scene/Components/Core/IDComponent.h"

LY_DISABLE_WARNINGS_PUSH
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

#include <nlohmann/json.hpp>
LY_DISABLE_WARNINGS_POP

namespace ly {
Level::Level(std::filesystem::path path, std::string name)
    : m_FilePath(std::move(path)), m_LevelName(std::move(name)) {}

/// \brief Load Serialized scene
Ref<scene::Scene> Level::LoadScene() const {
    auto scene          = MakeRef<scene::Scene>();
    auto& sceneRegistry = scene->GetRegistry();

    std::ifstream is(m_FilePath);
    if (!is.is_open()) {
        LY_CORE_ASSERT(false, "Failed to open scene file {}", m_FilePath.string());
    }

    cereal::JSONInputArchive archive(is);
    std::ifstream file(m_FilePath);
    auto data = nlohmann::ordered_json::parse(file);

    std::string levelName;
    archive(cereal::make_nvp("LevelName", levelName));

    int entityCount = 0;
    archive(cereal::make_nvp("EntityCount", entityCount));

    for (auto& [entityKey, entityData] : data.items()) {
        if (entityKey == "LevelName" || entityKey == "EntityCount") {
            continue;
        }

        archive.setNextName(entityKey.c_str());
        archive.startNode();
        [[maybe_unused]] auto entityFinishNode = MakeScopeGuard([&]() { archive.finishNode(); });

        entt::entity entity = sceneRegistry.create();
        sceneRegistry.emplace_or_replace<scene::DirtyComponent>(entity, scene::DirtyComponent());
        LY_CORE_LOG(LogType::INFO, "Created an entity {} with Id {}", entityKey, static_cast<uint32_t>(entity));

        for (auto& [compName, compData] : entityData.items()) {
            archive.setNextName(compName.c_str());
            archive.startNode();

            [[maybe_unused]] auto componentFinishNode = MakeScopeGuard([&]() { archive.finishNode(); });

            using namespace entt::literals;
            auto compType = entt::resolve(entt::hashed_string{ compName.c_str() });
            if (!compType) {
                archive.finishNode();
                continue;
            }

            // Check if entity can be loaded
            auto loadFunc = compType.func("enttLoad"_hs);
            if (!loadFunc) {
                LY_CORE_LOG(LogType::WARN, "No enttLoad function registered for '{}'", compName);
            }

            // construct instance of an entity
            auto instance = compType.construct();
            if (!instance) {
                LY_CORE_LOG(LogType::Error, "Could not construct instance");
                continue;
            }

            // Load the entity
            if (!loadFunc.invoke({}, entt::forward_as_meta(archive), instance.as_ref())) {
                LY_CORE_LOG(LogType::Error, "Failed to deserialize component '{}'", compName);
                continue;
            }

            // Update the deserialized entity in registry
            if (!compType.func("emplace"_hs)
                     .invoke({},
                             entt::forward_as_meta(sceneRegistry),
                             entt::forward_as_meta(entity),
                             entt::forward_as_meta(instance))) {
                LY_CORE_LOG(LogType::Error, "Emplace not called on component '{}'", compName);
            }
        }
    }
    LY_CORE_LOG(LogType::INFO, "Loading scene successfully completed");
    return std::move(scene);
}

void Level::SaveScene(scene::Scene& scene) const {
    auto& sceneRegistry = scene.GetRegistry();

    std::ofstream os(m_FilePath);
    cereal::JSONOutputArchive archive(os);

    archive(cereal::make_nvp("LevelName", m_LevelName));

    int entityCount = 0;
    for (auto entity : sceneRegistry.view<entt::entity>()) {
        entityCount++;
    }
    archive(cereal::make_nvp("EntityCount", entityCount));

    int val = 0;
    for (auto entity : sceneRegistry.view<entt::entity>()) {
        std::string entityKey = std::to_string(val);

        archive.setNextName(entityKey.c_str());
        archive.startNode();
        [[maybe_unused]] auto entityFinishNode = MakeScopeGuard([&]() { archive.finishNode(); });

        for (auto&& [typeId, storage] : sceneRegistry.storage()) {
            if (!storage.contains(entity)) continue;

            using namespace entt::literals;

            auto currentType   = entt::resolve(storage.type());
            auto serializeFunc = currentType.func("enttSave"_hs);
            if (!serializeFunc) {
                LY_CORE_LOG(LogType::INFO, "Serialize func does not exist for type {}", typeId);
            }

            auto instance = currentType.from_void(storage.value(entity));
            if (!instance) {
                LY_CORE_LOG(LogType::INFO, "Type instance could not be created for type {}", typeId);
                continue;
            }

            auto nameData = currentType.data("name"_hs);
            if (!nameData) {
                LY_CORE_LOG(ly::LogType::WARN, "Can't resolve component name of type {}", typeId);
                continue;
            }

            entt::meta_any compName = nameData.get(instance);
            archive.setNextName(compName.cast<const char*>());
            archive.startNode();
            [[maybe_unused]] auto componentFinishNode = MakeScopeGuard([&]() { archive.finishNode(); });

            if (!serializeFunc.invoke(instance, entt::forward_as_meta(archive))) {
                LY_CORE_LOG(ly::LogType::WARN, "Serialize function invoke failed for type {}", typeId);
            }
        }
        val++;
    }
    LY_CORE_LOG(LogType::INFO, "Scene saved");
}

}  // namespace ly