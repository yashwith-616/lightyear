#pragma once

#include <Lightyear/LightyearCore.h>
#include <Lightyear/Serialization/Serialization.h>

enum class BackendRendererAPI : uint8_t { OpenGL, Vulkan };

enum class OpenGLVersion : uint8_t {
    OpenGLv46,
    OpenGLv45,
};

static ly::GraphicContextVersion GetOpenGLVersion(OpenGLVersion version) {
    switch (version) {
        case OpenGLVersion::OpenGLv45:
            return { 4, 5 };
        case OpenGLVersion::OpenGLv46:
            return { 6, 7 };
        default:
            return { -1, -1 };
    }
}

namespace ly {

/// @brief Holds data related to engine specific requirements such as binary cache
/// resource search and other details
struct EngineSettings : SerializableContract {
    static constexpr Version version{ 1 };

    // Renderer API
    BackendRendererAPI rendererAPI;

    // The current openGL version
    OpenGLVersion openGLVersion;

    static void Serialize(StreamWriter& serializer, const EngineSettings& settings) {
        serializer.WriteVersion(settings.version);
        serializer.WriteRaw(static_cast<uint8_t>(settings.rendererAPI));
        serializer.WriteRaw(static_cast<uint8_t>(settings.openGLVersion));
    }


    static void Deserialize(StreamReader& deserializer, EngineSettings& settings) {
        Version currVersion = deserializer.ReadVersion();
        LY_CORE_ASSERT(currVersion == settings.version, "Version mismatch can't read file");

        settings.rendererAPI   = static_cast<BackendRendererAPI>(deserializer.ReadRaw<uint8_t>());
        settings.openGLVersion = static_cast<OpenGLVersion>(deserializer.ReadRaw<uint8_t>());
    }
};

}  // namespace ly