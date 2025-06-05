#pragma once

#include <imgui.h>
#include <imgui_internal.h>

class IEditorWidget {
public:
    ~IEditorWidget() = default;

    virtual void OnImGuiRender() = 0;
};