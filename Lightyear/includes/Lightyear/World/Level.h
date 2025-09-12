#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Core/Scene.h"

namespace ly {

class LIGHTYEAR_API Level {
public:
    explicit Level(std::filesystem::path path, std::string name);
    Level(const Level&)            = delete;
    Level& operator=(const Level&) = delete;
    Level(Level&&)                 = delete;
    Level& operator=(Level&&)      = delete;

    Scope<scene::Scene> LoadScene() const;
    void SaveScene(scene::Scene& scene) const;

private:
    std::filesystem::path m_FilePath;
    std::string m_LevelName;
};

}  // namespace ly
