#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Scene/Components/ComponentRegistry.h"

namespace ly::scene {

/// \brief Attaches a UUID to the component. All entity present in scene will have this component.
struct LIGHTYEAR_API IdComponent {
    Uuid id;

    IdComponent() : id(Uuid()) {}
    explicit IdComponent(Uuid const& id) : id(id) {}

    template <class Archive>
    void save(Archive& archive) {
        uint64_t value = id.get();
        archive(cereal::make_nvp("ID", value));
    }

    template <class Archive>
    void load(Archive& archive) {
        uint64_t value{};
        archive(cereal::make_nvp("ID", value));
        id = Uuid(value);
    }

    SERIALIZE_BODY(IdComponent)
};

}  // namespace ly::scene

REGISTER_COMPONENT(ly::scene::IdComponent, "IdComponent");

LY_DISABLE_WARNINGS_PUSH
#include <refl.hpp>
LY_DISABLE_WARNINGS_POP

// Will be removed
REFL_TYPE(ly::scene::IdComponent)
REFL_FIELD(id)
REFL_END