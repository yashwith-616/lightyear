#pragma once

#include <Lightyear/LightyearCore.h>

namespace ly
{

class ProjectSettings;
class EngineSettings;

/// @brief Loads the engine specific configuration from conf files into singleton
/// classes. These classes are useful in configuring project wide properties
class LIGHTYEAR_API SettingsManager
{
public:
    /// \brief Load all configuration files into memory
    static void loadConfigs();

    /// \brief Save all configuration files into save directory
    static void saveConfigs();
};

} // namespace ly
