#pragma once

#include <Lightyear/LightyearCore.h>

namespace ly {

/// @brief Project specific properties are placed in following class. Will be added to
/// in UI for easier access to these properties
struct LIGHTYEAR_API ProjectSettings {
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
};

}  // namespace ly