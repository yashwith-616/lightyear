#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include "Lightyear.h"

class IEditorPanel {
public:
    virtual ~IEditorPanel()      = default;
    virtual void OnImGuiRender() = 0;

protected:
    std::string m_Name{"none"};
    bool m_bIsVisible{ false };
};