#pragma once

#include <Lightyear.h>

class GlobalEditorContext {
public:
    ly::Ref<ly::scene::Scene> ActiveScene;
    ly::Ref<ly::renderer::Framebuffer> SceneFramebuffer;
    ly::Ref<ly::scene::SceneRuntime> SceneRuntime;
};