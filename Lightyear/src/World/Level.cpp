#include "Lightyear/World/Level.h"
#include "Lightyear/Serialization/Text/YamlTextSerialization.h"

namespace ly {

Level::Level(std::filesystem::path path, std::string name)
    : m_FilePath(std::move(path)), m_LevelName(std::move(name)) {}

Scope<scene::Scene> Level::LoadScene() const {
    auto scene        = ly::MakeScope<scene::Scene>(scene::Scene());
    auto deserializer = YamlTextDeserializer(m_FilePath);
    return std::move(scene);
}

void Level::SaveScene(scene::Scene& scene) const {
    auto serializer     = YamlTextSerializer(m_FilePath);
    auto& sceneRegistry = scene.GetRegistry();

    serializer.BeginObject(m_LevelName);
    int val = 0;
    for (auto entity : sceneRegistry.view<entt::entity>()) {
        serializer.BeginObject(std::to_string(val));
        for (auto&& [typeId, storage] : sceneRegistry.storage()) {
            if (!storage.contains(entity)) {
                continue;
            }

            using namespace entt::literals;

            auto currentType = entt::resolve(storage.type());
            if (auto serializeFunc = currentType.func("Serialize"_hs); serializeFunc) {
                auto instance = currentType.from_void(storage.value(entity));
                serializeFunc.invoke({}, entt::forward_as_meta(serializer), instance);
            }
        }
        serializer.EndObject();
        val++;
    }
    serializer.EndObject();
    serializer.SaveToFile();
    LY_CORE_LOG(LogType::INFO, "Scene saved");
}

}  // namespace ly
