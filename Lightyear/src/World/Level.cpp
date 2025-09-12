#include "Lightyear/World/Level.h"
#include "Lightyear/Serialization/Text/YamlTextSerialization.h"

namespace ly {

Level::Level(std::filesystem::path path, std::string name)
    : m_FilePath(std::move(path)), m_LevelName(std::move(name)) {}

Scope<scene::Scene> Level::LoadScene() const {
    /// TODO: Use unqiue pointer here
    auto scene = ly::MakeScope<scene::Scene>(scene::Scene());
    auto deserializer = YamlTextDeserializer(m_FilePath);
    return std::move(scene);
}

void Level::SaveScene(scene::Scene& scene) const {
    auto serializer = YamlTextSerializer(m_FilePath);

    auto& sceneRegistry = scene.GetRegistry();
    sceneRegistry.each([&] (auto& entityId) {

    });
}

}  // namespace ly
