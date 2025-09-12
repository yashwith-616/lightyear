#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/Core/SingletonComponent.h"

namespace ly::scene {
/**
 * Indicates the following is the main camera and is used for rendering the scene during game runtime
 */
struct LIGHTYEAR_API MainCameraTag final : SingletonComponent<MainCameraTag> {};

/**
 * The editor camera tag is used for indicating the editor camera and used when rendering the scene inside the editor.
 */
struct LIGHTYEAR_API EditorCameraTag final : SingletonComponent<EditorCameraTag> {};

}  // namespace ly::scene

// --- Reflection Property ---
LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::MainCameraTag)
REFL_END