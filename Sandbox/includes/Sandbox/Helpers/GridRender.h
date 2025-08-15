#pragma once

#include "Lightyear.h"

const std::unordered_map<ly::renderer::ShaderType, ly::CPath> g_GridShader = {
    { ly::renderer::ShaderType::VERTEX, ASSET_DIR "/Shaders/Vertex/S_Grid.vert" },
    { ly::renderer::ShaderType::FRAGMENT, ASSET_DIR "/Shaders/Fragment/S_Grid.frag" }
};

const std::unordered_map<ly::renderer::ShaderType, ly::CPath> g_DefaultShader = {
    { ly::renderer::ShaderType::VERTEX, ASSET_DIR "/Shaders/Vertex/S_Default.vert" },
    { ly::renderer::ShaderType::FRAGMENT, ASSET_DIR "/Shaders/Fragment/S_Default.frag" }
};

class GridRender {
public:
    GridRender();
    ~GridRender() = default;

    void Render() const;

private:
    ly::Ref<ly::renderer::VertexArray> m_GridVAO;
    ly::Ref<ly::renderer::Shader> m_GridShader;
};