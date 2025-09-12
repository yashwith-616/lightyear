#pragma once

#include <Lightyear/LightyearCore.h>
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly {

/// @brief Project specific properties are placed in following class. Will be added to
/// in UI for easier access to these properties
struct LIGHTYEAR_API ProjectSettings : SerializableContract {
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

    static void Serialize(TextSerializer& serializer, const ProjectSettings& settings) {
        serializer.Write("name", settings.name);
        serializer.Write("description", settings.description);
        serializer.Write("project_version", settings.projectVersion);
        serializer.Write("asset_directory", settings.assetDirectory.string());
        serializer.Write("cache_directory", settings.cacheDirectory.string());
        serializer.Write("config_directory", settings.configDirectory.string());
        serializer.Write("saved_directory", settings.savedDirectory.string());
    }

    static void Deserialize(TextDeserializer& deserializer, ProjectSettings& settings) {
        deserializer.Read("name", settings.name);
        deserializer.Read("description", settings.description);
        deserializer.Read("project_version", settings.projectVersion);

        std::string directory;
        deserializer.Read("asset_directory", directory);
        settings.assetDirectory = directory;

        deserializer.Read("cache_directory", directory);
        settings.cacheDirectory = directory;

        deserializer.Read("config_directory", directory);
        settings.configDirectory = directory;

        deserializer.Read("saved_directory", directory);
        settings.savedDirectory = directory;
    }
};

}  // namespace ly