#pragma once

#include <imgui.h>
#include "Sandbox/Editor/Workspace/IEditorWorkspace.h"

class ESceneWorkspace : IEditorWorkspace {
public:
    ESceneWorkspace(std::string_view name) : IEditorWorkspace(name) {}
    ~ESceneWorkspace() = default;

    virtual void OnAttach(ly::Ref<GlobalEditorContext> globalContext) override;
    virtual void OnEvent(ly::Event& event) override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEditorUpdate() override;
    virtual void OnImGuiRender() override;

protected:
    enum class EEditorPanel : uint8_t { VIEWPORT = 0, SCENE_GRAPH, INSPECTOR, PLACE_ACTOR, MAX };

    ImGuiID m_DockspaceID{};

    inline bool IsDockspaceInitialized() { return m_bIsMinimized; }
    void SetupDockspace();
    const char* GetPanelTitle(EEditorPanel editorPanel);

private:
    bool m_bIsInitiatlized{ false };
};