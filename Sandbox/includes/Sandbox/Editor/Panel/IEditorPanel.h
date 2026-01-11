#pragma once

#include "Lightyear.h"

LY_DISABLE_WARNINGS_PUSH
#include <imgui.h>
#include <imgui_internal.h>
LY_DISABLE_WARNINGS_POP

class IEditorPanel
{
public:
    IEditorPanel(std::string name) : m_Name(std::move(name)) {}
    virtual ~IEditorPanel() = default;
    virtual void OnImGuiRender() = 0;

protected:
    std::string m_Name{"NULL"};
    bool m_bIsVisible{false};
};
