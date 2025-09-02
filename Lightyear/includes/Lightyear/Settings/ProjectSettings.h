#pragma once

#include <Lightyear/LightyearCore.h>
#include "Lightyear/Serialization/Serialization.h"

namespace ly {

/// @brief Project specific properties are placed in following class. Will be added to
/// in UI for easier access to these properties
struct LIGHTYEAR_API ProjectSettings : SerializableContract {
    static constexpr Version version{ 1 };

    // Name of the project
    std::string name;

    // Project description
    std::string description;

    // Version of the project
    std::string projectVersion;

    // Search assets in the following directory
    std::filesystem::path assetDirectory;

    // All cache generated for assets is pushed here
    std::filesystem::path cacheDirectory;

    // All config files for a project is stored in the following file
    std::filesystem::path configDirectory;

    // The logs or screenshots and other persistent data is added here
    std::filesystem::path savedDirectory;

    static void Serialize(StreamWriter& serializer, const ProjectSettings& settings) {
        serializer.WriteVersion(ProjectSettings::version);
        serializer.WriteString(settings.name);
        serializer.WriteString(settings.description);
        serializer.WriteString(settings.projectVersion);
        serializer.WriteString(settings.assetDirectory.string());
        serializer.WriteString(settings.cacheDirectory.string());
        serializer.WriteString(settings.configDirectory.string());
        serializer.WriteString(settings.savedDirectory.string());
    }

    static void Deserialize(StreamReader& deserializer, ProjectSettings& settings) {
        const Version currVersion = deserializer.ReadVersion();
        LY_CORE_ASSERT(currVersion == settings.version, "Version mismatch can't read file");
        settings.name            = deserializer.ReadString();
        settings.description     = deserializer.ReadString();
        settings.projectVersion  = deserializer.ReadString();
        settings.assetDirectory  = deserializer.ReadString();
        settings.cacheDirectory  = deserializer.ReadString();
        settings.configDirectory = deserializer.ReadString();
        settings.savedDirectory  = deserializer.ReadString();
    }
};

}  // namespace ly