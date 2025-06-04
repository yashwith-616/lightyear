#pragma once

#include "Lightyear.h"

class EditorCamera;

class GridRender {
public:
    GridRender();
    ~GridRender() = default;

    void OnUpdate(ly::Ref<EditorCamera> camera);
    void Render();

private:
    ly::Scope<ly::renderer::VertexArray> m_GridVAO;
    ly::Scope<ly::renderer::Shader> m_GridShader;
};