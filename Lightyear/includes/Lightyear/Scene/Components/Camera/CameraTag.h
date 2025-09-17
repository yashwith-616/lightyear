#pragma once

#include "Lightyear/Scene/Components/ComponentRegistry.h"
#include "Lightyear/Scene/Components/Core/SingletonComponent.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly::scene {

/// \brief Indicates the following is the main camera and is used for rendering the scene during game runtime
struct LIGHTYEAR_API MainCameraTag final : SingletonComponent<MainCameraTag>, SerializableContract {
    static void Serialize(TextSerializer& serializer, const MainCameraTag& value) {
        serializer.BeginObject("MainCameraTag");
        serializer.EndObject();
    }

    static void Deserialize(TextDeserializer& serializer, MainCameraTag& value) {}
};

/// \brief The editor camera tag is used for indicating the editor camera and used when rendering the scene inside the
/// editor.
struct LIGHTYEAR_API EditorCameraTag final : SingletonComponent<EditorCameraTag>, SerializableContract {
    static void Serialize(TextSerializer& serializer, const EditorCameraTag& value) {
        serializer.BeginObject("EditorCameraTag");
        serializer.EndObject();
    }
    static void Deserialize(TextDeserializer& deserializer, EditorCameraTag& value) {
        deserializer.BeginObject("MainCameraTag");
        deserializer.EndObject();
    }
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