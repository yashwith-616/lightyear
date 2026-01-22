#pragma once

#include "Lightyear/LightyearCore.h"

namespace ly::scene
{

enum class ERenderVisibilityFlags : uint8_t
{
    None = bit(0),
    Hidden = bit(1),
    EditorHidden = bit(2),
    HiddenEverywhere = bit(3)
};

struct LIGHTYEAR_API RenderComponent
{
    uint8_t visibilityFlags = static_cast<uint8_t>(ERenderVisibilityFlags::None);

    [[nodiscard]] bool isHidden() const
    {
        return (visibilityFlags & static_cast<uint8_t>(ERenderVisibilityFlags::Hidden)) != 0;
    }

    [[nodiscard]] bool isEditorHidden() const
    {
        return (visibilityFlags & static_cast<uint8_t>(ERenderVisibilityFlags::EditorHidden)) != 0;
    }
};

} // namespace ly::scene
