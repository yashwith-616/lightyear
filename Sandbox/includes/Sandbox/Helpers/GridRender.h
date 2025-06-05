#pragma once

#include "Lightyear.h"

class EditorCamera;

class GridRender {
public:
    GridRender();
    ~GridRender() = default;

    void Render();

private:
    ly::Ref<ly::renderer::VertexArray> m_GridVAO;
    ly::Ref<ly::renderer::Shader> m_GridShader;
};