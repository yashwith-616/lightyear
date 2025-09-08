#include <Lightyear/Settings/SettingsManager.h>

#include <Lightyear/Settings/EngineSettings.h>
#include <Lightyear/Settings/ProjectSettings.h>

#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly {

void SettingsManager::LoadConfigs() {
    // load JSON/INI/etc. into g_project, g_engine, g_editor
}
void SettingsManager::SaveConfigs() {
    // dump settings back to file
}

}  // namespace ly
