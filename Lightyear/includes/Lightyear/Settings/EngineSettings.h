#pragma once

#include <Lightyear/LightyearCore.h>
#include <Lightyear/Serialization/Text/TextSerialization.h>

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

/// \brief Holds data related to engine specific requirements such as binary cache
/// resource search and other details
struct EngineSettings : SerializableContract {
    static constexpr Version version{ 1 };

    // Renderer API
    BackendRendererAPI rendererAPI;

    // The current openGL version
    OpenGLVersion openGLVersion;

    static void Serialize(TextSerializer& serializer, const EngineSettings& settings) {
        serializer.Write("version", version);
        serializer.Write("renderer_api", static_cast<uint8_t>(settings.rendererAPI));
        serializer.Write("opengl_version", static_cast<uint8_t>(settings.openGLVersion));
    }

    static void Deserialize(TextDeserializer& deserializer, EngineSettings& settings) {
        Version currVersion{ 0 };
        deserializer.Read("version", currVersion);
        LY_CORE_ASSERT(currVersion == settings.version, "Version mismatch can't read file");

        uint8_t enumVal{ 0 };
        deserializer.Read("renderer_api", enumVal);
        settings.rendererAPI = static_cast<BackendRendererAPI>(enumVal);

        deserializer.Read("opengl_version", enumVal);
        settings.openGLVersion = static_cast<OpenGLVersion>(enumVal);
    }
};

}  // namespace ly