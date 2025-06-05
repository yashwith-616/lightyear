#pragma once

#include "Lightyear.h"
#include <imgui.h>
#include <imgui_internal.h>

class IEditorPanel {
public:
    virtual ~IEditorPanel() = default;
    virtual void OnImGuiRender() = 0;
};