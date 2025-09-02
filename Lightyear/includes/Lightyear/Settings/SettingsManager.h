#pragma once

#include <Lightyear/LightyearCore.h>

namespace ly {

class ProjectSettings;
class EngineSettings;

/// @brief Loads the engine specific configuration from conf files into singleton
/// classes. These classes are useful in configuring project wide properties
class LIGHTYEAR_API SettingsManager {
public:
    static ProjectSettings& Project();
    static EngineSettings& Engine();

    /// \brief Load all configuration files into memory
    static void LoadConfigs();

    /// \brief Save all configuration files into save directory
    static void SaveConfigs();

private:
    static ProjectSettings g_ProjectSettings;
    static EngineSettings g_EngineSettings;
};

}  // namespace ly