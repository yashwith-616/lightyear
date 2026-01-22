#pragma once

#include <Lightyear.h>
#include "Sandbox/Editor/Comman/GlobalEditorContext.h"
#include "Sandbox/Editor/Panel/IEditorPanel.h"

class IEditorWorkspace
{
public:
    IEditorWorkspace(std::string name) : m_name(std::move(name)) {}
    virtual ~IEditorWorkspace() = default;

    virtual void onAttach(ly::ref<GlobalEditorContext> globalContext) = 0;
    virtual void onEvent(ly::Event& event) = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onEditorUpdate() = 0;
    virtual void onImGuiRender() = 0;

protected:
    std::string m_name{"NONE"};
    std::vector<IEditorPanel> m_editorPanels{};
    ly::ref<GlobalEditorContext> m_globalEditorContext;

    bool m_isMinimized{false};
};
