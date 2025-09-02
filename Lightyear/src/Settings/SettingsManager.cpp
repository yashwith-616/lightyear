#include <Lightyear/Settings/SettingsManager.h>

#include <Lightyear/Settings/EngineSettings.h>
#include <Lightyear/Settings/ProjectSettings.h>

#include "Lightyear/Serialization/FileSerializer.h"

namespace ly {

ProjectSettings& SettingsManager::Project() {
    return g_ProjectSettings;
}

EngineSettings& SettingsManager::Engine() {
    return g_EngineSettings;
}

void SettingsManager::LoadConfigs() {
    // load JSON/INI/etc. into g_project, g_engine, g_editor
}
void SettingsManager::SaveConfigs() {
    // dump settings back to file
}

}  // namespace ly
