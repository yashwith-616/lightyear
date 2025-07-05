#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include "Lightyear.h"

class IEditorPanel {
public:
    IEditorPanel(std::string_view name) { m_Name = std::string(name); }
    virtual ~IEditorPanel()      = default;
    virtual void OnImGuiRender() = 0;

protected:
    std::string m_Name{ "none" };
    bool m_bIsVisible{ false };
};