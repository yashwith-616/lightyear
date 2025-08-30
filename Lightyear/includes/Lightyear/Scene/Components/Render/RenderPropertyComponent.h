#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene {

enum class ERenderVisibilityFlags : uint8_t {
    None             = BIT(0),
    Hidden           = BIT(1),
    EditorHidden     = BIT(2),
    HiddenEverywhere = BIT(3)
};

struct LIGHTYEAR_API RenderComponent {
    uint8_t VisibilityFlags = static_cast<uint8_t>(ERenderVisibilityFlags::None);

    [[nodiscard]] bool IsHidden() const {
        return VisibilityFlags & static_cast<uint8_t>(ERenderVisibilityFlags::Hidden);
    }

    [[nodiscard]] bool IsEditorHidden() const {
        return VisibilityFlags & static_cast<uint8_t>(ERenderVisibilityFlags::EditorHidden);
    }
};

}  // namespace ly::scene