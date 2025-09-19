#pragma once

#include "Lightyear/Scene/Components/ComponentRegistry.h"
#include "Lightyear/Scene/Components/Core/SingletonComponent.h"

namespace ly::scene {

/// \brief Indicates the following is the main camera and is used for rendering the scene during game runtime
struct LIGHTYEAR_API MainCameraTag final : SingletonComponent<MainCameraTag> {
    template <class Archive>
    void serialize(Archive& archive) {
        bool present = true;
        archive(cereal::make_nvp("MainCameraTag", present));
    }

    SERIALIZE_BODY(MainCameraTag)
};

/// \brief The editor camera tag is used for indicating the editor camera and used when rendering the scene inside the
/// editor.
struct LIGHTYEAR_API EditorCameraTag final : SingletonComponent<EditorCameraTag> {
    template <class Archive>
    void serialize(Archive& archive) {
        bool present = true;
        archive(cereal::make_nvp("EditorCameraTag", present));
    }

    SERIALIZE_BODY(EditorCameraTag)
};

}  // namespace ly::scene

// --- Reflection ---
REGISTER_COMPONENT(ly::scene::MainCameraTag, "MainCameraTag");
REGISTER_COMPONENT(ly::scene::EditorCameraTag, "EditorCameraTag");

// --- Reflection Property ---
LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

REFL_TYPE(ly::scene::MainCameraTag)
REFL_END