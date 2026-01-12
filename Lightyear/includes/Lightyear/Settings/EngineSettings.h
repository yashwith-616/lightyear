#pragma once

#include <Lightyear/LightyearCore.h>

enum class BackendRendererApi : uint8_t { OpenGl, Vulkan };

enum class OpenGlVersion : uint8_t {
    OpenGLv46,
    OpenGLv45,
};

static ly::GraphicContextVersion getOpenGlVersion(OpenGlVersion version) {
    switch (version) {
        case OpenGlVersion::OpenGLv45:
            return { 4, 5 };
        case OpenGlVersion::OpenGLv46:
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
    BackendRendererApi rendererApi;

    // The current openGL version
    OpenGlVersion openGlVersion;
};

}  // namespace ly