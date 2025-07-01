#include "Sandbox/Editor/Workspace/ESceneWorkspace.h"

void ESceneWorkspace::OnAttach(ly::Ref<GlobalEditorContext> globalContext) {
    m_GlobalEditorContext = globalContext;
}

void ESceneWorkspace::OnEvent(ly::Event& event) {}

void ESceneWorkspace::OnUpdate(float deltaTime) {}

void ESceneWorkspace::OnEditorUpdate() {}

void ESceneWorkspace::OnImGuiRender() {
    if (!IsDockspaceInitialized()) {
        SetupDockspace();
    }

    ImGui::DockSpace(m_DockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_None);
}

/**
 * TODO: Currently setup for a fixed layout
 *
 * 1. Need to use EditorPreset so the layout can be changed as per requirement
 * 2. EditorLayout need to be cached based on requirement
 * 3. All the panels eligible this Workspace needs to be added to menuBar > windows
 * 4. Layouts for all window will be distributed to all window before hand.
 * 5. Panel will have property to hide them. Panel need an enum based dirty flag
 */
void ESceneWorkspace::SetupDockspace() {
    ImGui::DockBuilderRemoveNode(m_DockspaceID);
    ImGui::DockBuilderAddNode(m_DockspaceID, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(m_DockspaceID, ImGui::GetMainViewport()->Size);

    ImGuiID left, right, topRight, bottomRight;
    ImGui::DockBuilderSplitNode(m_DockspaceID, ImGuiDir::ImGuiDir_Right, 0.2, &right, &left);
    ImGui::DockBuilderSplitNode(left, ImGuiDir::ImGuiDir_Down, 0.5, &bottomRight, &topRight);

    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::VIEWPORT), left);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::SCENE_GRAPH), topRight);
    ImGui::DockBuilderDockWindow(GetPanelTitle(EEditorPanel::INSPECTOR), bottomRight);

    ImGui::DockBuilderFinish(m_DockspaceID);

    m_bIsInitiatlized = true;
}

const char* ESceneWorkspace::GetPanelTitle(EEditorPanel editorPanel) {
    switch (editorPanel) {
        case EEditorPanel::VIEWPORT:
            return "Viewport";
        case EEditorPanel::SCENE_GRAPH:
            return "SceneGraph";
        case EEditorPanel::INSPECTOR:
            return "Inspector";
        case EEditorPanel::PLACE_ACTOR:
            return "PlaceActor";
        default:
            return "None";
    };
}