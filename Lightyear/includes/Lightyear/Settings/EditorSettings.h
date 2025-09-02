#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Serialization/Serialization.h"

namespace ly {

struct LIGHTYEAR_API EditorSettings : SerializableContract {
    static constexpr Version version{ 1 };

    // Window default API
    WindowSize startupWindowSize{ WindowSize(1280, 720) };

    // The window title
    std::string windowTitle{ "Lightyear" };

    // Aspect ratio of the window
    float aspectRatio{ kDefaultAspectRatio };

    // The default FOV
    float fovRadians{ kDefaultFOVRadians };

    // Near clip of the scene
    float nearClip{ kDefaultNearClip };

    // Far clip of the scene
    float farClip{ kDefaultFarClip };

    float defaultFrameTime{ kDefaultFrametime };

    static void Serialize(StreamWriter& serializer, const EditorSettings& settings) {
        serializer.WriteVersion(settings.version);
        serializer.WriteRaw(settings.startupWindowSize.width);
        serializer.WriteRaw(settings.startupWindowSize.height);
        serializer.WriteString(settings.windowTitle);
        serializer.WriteRaw(settings.aspectRatio);
        serializer.WriteRaw(settings.fovRadians);
        serializer.WriteRaw(settings.nearClip);
        serializer.WriteRaw(settings.farClip);
        serializer.WriteRaw(settings.defaultFrameTime);
    }

    static void Deserialize(StreamReader& deserializer, EditorSettings& settings) {
        Version currVersion = deserializer.ReadVersion();
        LY_CORE_ASSERT(currVersion == settings.version, "Version mismatch can't read file");

        settings.startupWindowSize.width  = deserializer.ReadRaw<int32_t>();
        settings.startupWindowSize.height = deserializer.ReadRaw<int32_t>();
        settings.windowTitle              = deserializer.ReadString();
        settings.aspectRatio              = deserializer.ReadRaw<float>();
        settings.fovRadians               = deserializer.ReadRaw<float>();
        settings.nearClip                 = deserializer.ReadRaw<float>();
        settings.farClip                  = deserializer.ReadRaw<float>();
        settings.defaultFrameTime         = deserializer.ReadRaw<float>();
    }
};

}  // namespace ly
