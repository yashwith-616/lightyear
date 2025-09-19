#include "Lightyear/World/Level.h"

LY_DISABLE_WARNINGS_PUSH
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>
LY_DISABLE_WARNINGS_POP

namespace ly {

Level::Level(std::filesystem::path path, std::string name)
    : m_FilePath(std::move(path)), m_LevelName(std::move(name)) {}

Ref<scene::Scene> Level::LoadScene() const {
    auto scene          = MakeRef<scene::Scene>();
    auto& sceneRegistry = scene->GetRegistry();

    std::ifstream is(m_FilePath);
    if (!is.is_open()) {
        LY_CORE_ASSERT(false, "Failed to open scene file {}", m_FilePath.string());
    }

    cereal::JSONInputArchive archive(is);

    std::string levelName;
    archive(cereal::make_nvp("LevelName", levelName));

    int entityCount = 0;
    archive(cereal::make_nvp("EntityCount", entityCount));

    for (int i = 0; i < entityCount; i++) {
        std::string entityKey = std::to_string(i);

        archive.setNextName(entityKey.c_str());
        archive.startNode();

        entt::entity entity = sceneRegistry.create();

        for (auto&& [typeId, storage] : sceneRegistry.storage()) {
            using namespace entt::literals;
            auto currentType = entt::resolve(storage.type());

            if (auto loadFunc = currentType.func("enttLoad"_hs); loadFunc) {
                auto instance = currentType.construct();
                if (!instance) {
                    LY_CORE_LOG(LogType::Error, "Could not construct instance");
                }

                auto nameData = currentType.data("name"_hs);
                if (!nameData) {
                    LY_CORE_LOG(ly::LogType::INFO, "Can't resolve name");
                    continue;
                }
                entt::meta_any enttMetaValue = nameData.get(instance);
                const char* compName         = enttMetaValue.cast<const char*>();

                // TODO: If compName exists in document only then invoke, else not require

                // Deserialize into component instance
                if (!loadFunc.invoke({}, entt::forward_as_meta(archive), instance)) {
                    LY_CORE_LOG(LogType::Error, "Could not call deserialize");
                }

                // Emplace into registry
                if (!currentType.func("emplace"_hs)
                         .invoke({},
                                 entt::forward_as_meta(sceneRegistry),
                                 entt::forward_as_meta(entity),
                                 entt::forward_as_meta(instance))) {
                    LY_CORE_LOG(LogType::Error, "Could not emplace json values");
                }
            }
        }

        archive.finishNode();
    }
    LY_CORE_LOG(LogType::INFO, "Scene loaded");
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
        archive.startNode();  // push a new JSON object scope

        {
            for (auto&& [typeId, storage] : sceneRegistry.storage()) {
                if (!storage.contains(entity)) continue;

                using namespace entt::literals;

                auto currentType = entt::resolve(storage.type());
                if (auto serializeFunc = currentType.func("enttSave"_hs); serializeFunc) {
                    LY_CORE_LOG(ly::LogType::INFO, "Serializing function");

                    auto instance = currentType.from_void(storage.value(entity));
                    if (!instance) {
                        continue;
                    }

                    auto nameData = currentType.data("name"_hs);
                    if (!nameData) {
                        LY_CORE_LOG(ly::LogType::INFO, "Can't resolve name");
                        continue;
                    }
                    entt::meta_any compName = nameData.get(instance);
                    archive.setNextName(compName.cast<const char*>());
                    archive.startNode();

                    if (!serializeFunc.invoke(instance, entt::forward_as_meta(archive))) {
                        LY_CORE_LOG(ly::LogType::INFO, "Invoke failed to match arguments");
                    }

                    archive.finishNode();
                }
            }
        }
        archive.finishNode();  // close JSON object
        val++;
    }
    LY_CORE_LOG(LogType::INFO, "Scene saved");
}

/*void Level::SaveScene(scene::Scene& scene) const {
    const entt::registry& sceneRegistry = scene.GetRegistry();

    std::ofstream os(m_FilePath);
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp("LevelName", m_LevelName));

    using namespace entt::literals;
    for (auto&& [typeId, storage] : sceneRegistry.storage()) {
        auto type = entt::resolve(typeId);
        type.invoke("enttSave"_hs, {}, entt::forward_as_meta(archive), entt::forward_as_meta(sceneRegistry));
    }
}*/

}  // namespace ly
