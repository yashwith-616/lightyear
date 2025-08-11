#pragma once

#include <Lightyear.h>

class GlobalEditorContext {
public:
    ly::Ref<ly::scene::Scene> ActiveScene;
    uint32_t SceneFramebufferId;
};