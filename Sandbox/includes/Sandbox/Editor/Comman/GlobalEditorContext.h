#pragma once

#include <Lightyear.h>

class GlobalEditorContext
{
public:
    ly::ref<ly::scene::Scene> activeScene;
    ly::ref<ly::renderer::Framebuffer> sceneFramebuffer;
    ly::ref<ly::scene::SceneRuntime> sceneRuntime;
};
