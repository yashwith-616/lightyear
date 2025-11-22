#pragma once

#include <Lightyear/LightyearCore.h>

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
struct EngineSettings {
    // Renderer API
    BackendRendererAPI rendererAPI;

    // The current openGL version
    OpenGLVersion openGLVersion;
};

}  // namespace ly