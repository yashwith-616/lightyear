#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene {

/// \brief Attaches a UUID to the component. All entity present in scene will have this component.
struct LIGHTYEAR_API IDComponent {
    UUID ID;

    IDComponent() : ID(UUID()) {}
    explicit IDComponent(const UUID& id) : ID(id) {}

    template <class Archive>
    void save(Archive& archive) {
        uint64_t value = ID.Get();
        archive(cereal::make_nvp("ID", value));
    }

    template <class Archive>
    void load(Archive& archive) {
        uint64_t value{};
        archive(cereal::make_nvp("ID", value));
        ID = UUID(value);
    }

    // SERIALIZE_BODY(IDComponent)
    const char* Name = "IDComponent";

    void SaveJson(cereal::JSONOutputArchive& archive) { save(archive); }

    static void LoadJson(cereal::JSONInputArchive& archive, IDComponent& component) { component.load(archive); }
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::IDComponent, "IDComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::IDComponent)
REFL_FIELD(ID)
REFL_END