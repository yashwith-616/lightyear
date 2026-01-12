#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Core/Scene.h"

namespace ly {

class LIGHTYEAR_API Level {
public:
    explicit Level(std::filesystem::path path, std::string name);
    Level(Level const&)            = delete;
    Level& operator=(Level const&) = delete;
    Level(Level&&)                 = delete;
    Level& operator=(Level&&)      = delete;

    ref<scene::Scene> loadScene() const;
    void saveScene(scene::Scene& scene) const;

private:
    std::filesystem::path m_filePath;
    std::string m_levelName;
};

}  // namespace ly
