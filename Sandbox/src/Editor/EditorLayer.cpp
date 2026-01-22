#include "Sandbox/Editor/EditorLayer.h"

#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Helpers/GridRender.h"

LY_DISABLE_WARNINGS_PUSH
#include <imgui.h>
#include <imgui_internal.h>
LY_DISABLE_WARNINGS_POP

constexpr auto k_clearColor = glm::vec4(0.13, 0.13, 0.13, 1.0);
auto const k_cameraSavePath = std::filesystem::path(SAVED_DIR "/camera.yaml");
auto const k_sceneSavePath = std::filesystem::path(SAVED_DIR "/scene.yaml");
auto const k_sceneSavePathJson = std::filesystem::path(SAVED_DIR "/scene.json");
auto const k_sceneSavePathJsonTest = std::filesystem::path(SAVED_DIR "/scene_test.json");

namespace renderer = ly::renderer;

EditorLayer::EditorLayer() : Layer("Editor") {}

void EditorLayer::onAttach()
{
    loadScene();
    m_sceneRuntime = ly::makeRef<ly::scene::SceneRuntime>(m_scene.get());
    m_sceneRuntime->initialize();

#pragma region Framebuffer Init
    // TODO: Framebuffer must be moved to a separate Framebuffer pool and must be accessed by the
    // world. The world needs to be responsible for rendering.

    renderer::FramebufferSpecification spec;
    spec.width = ly::k_defaultWindowSize.x;
    spec.height = ly::k_defaultWindowSize.y;
    m_framebuffer = renderer::Framebuffer::create(spec);
#pragma endregion

#pragma region Shaders and Textures
    // TODO: Texture and Shader will be made as part of the Entity.
    // Will be removed from here in the future implementation

    m_texture = renderer::Texture2D::create(ASSET_DIR "/Textures/T_Grid.png");
    m_shader = renderer::Shader::create("ShaderBg", k_defaultShader);
#pragma endregion

    /// TODO: Remove when asset-registry is done. Add mesh and render component to cube.
    ly::scene::Entity entity = m_scene->findEntityByName("Cube");
    entity.addComponent<ly::scene::MeshComponent>(Geometry::getCube(), m_shader, m_texture);
    entity.addComponent<ly::scene::RenderComponent>();

    m_sceneRuntime->onViewportResize(glm::uvec2(spec.width, spec.height));
    m_sceneRuntime->setSceneExecState(ly::scene::SceneRuntimeMode::Play);

#pragma region Inti Scene and Panels
    m_sceneWorkspace = ly::makeScope<ESceneWorkspace>("SceneWorkspace");
    m_editorContext = ly::makeRef<GlobalEditorContext>();
    m_editorContext->activeScene = m_scene;
    m_editorContext->sceneFramebuffer = m_framebuffer;
    m_editorContext->sceneRuntime = m_sceneRuntime;
    m_sceneWorkspace->onAttach(m_editorContext);
#pragma endregion
}

void EditorLayer::onDetach() { saveScene(); }

void EditorLayer::onEditorRender()
{
    m_sceneWorkspace->onEditorUpdate();
    m_sceneWorkspace->onImGuiRender();

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay"))
    {
        ImGui::Text("IsRunning: %s", m_sceneRuntime->isRunning() ? "true" : "false");
        ImGui::Text("IsPaused: %s", m_sceneRuntime->isPaused() ? "true" : "false");
    }
    ImGui::End();

    ImGui::ShowDemoWindow();
}

void EditorLayer::onUpdate(float deltaTime)
{
    m_framebuffer->bind();
    renderer::RenderCommand::clear();
    renderer::RenderCommand::setClearColor(k_clearColor);

    // m_SceneRuntime->OnStart();
    m_sceneRuntime->onUpdate(deltaTime);
    // m_SceneRuntime->OnEnd();

    m_framebuffer->unbind();
}

void EditorLayer::saveScene() const
{
    LY_LOG(ly::LogType::Info, "Saving scene");
    auto* level = new ly::Level(k_sceneSavePathJson, "ALevel");
    level->saveScene(*m_scene);
}
void EditorLayer::loadScene()
{
    auto* level = new ly::Level(k_sceneSavePathJson, "ALevel");
    m_scene = level->loadScene();
}
