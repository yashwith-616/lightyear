#pragma once

#include <Lightyear.h>
#include <string_view>
#include "Sandbox/Editor/Comman/GlobalEditorContext.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"

class IEditorWorkspace {
public:
    IEditorWorkspace(std::string_view name) { m_Name = std::string(name); }

    virtual void OnAttach(ly::Ref<GlobalEditorContext> globalContext) = 0;
    virtual void OnEvent(ly::Event& event)                            = 0;
    virtual void OnUpdate(float deltaTime)                            = 0;
    virtual void OnEditorUpdate()                                     = 0;
    virtual void OnImGuiRender()                                      = 0;

protected:
    std::string m_Name{ "none" };
    std::vector<IEditorPanel> m_EditorPanels{};
    ly::Ref<GlobalEditorContext> m_GlobalEditorContext;

    bool m_bIsMinimized{ false };
};