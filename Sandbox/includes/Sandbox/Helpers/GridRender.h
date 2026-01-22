#pragma once

#include "Lightyear.h"

std::unordered_map<ly::renderer::ShaderType, ly::cPath> const k_gridShader = {
    {ly::renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/S_Grid.vert"},
    {ly::renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/S_Grid.frag"}};

std::unordered_map<ly::renderer::ShaderType, ly::cPath> const k_defaultShader = {
    {ly::renderer::ShaderType::Vertex, ASSET_DIR "/Shaders/Vertex/S_Default.vert"},
    {ly::renderer::ShaderType::Fragment, ASSET_DIR "/Shaders/Fragment/S_Default.frag"}};

class GridRender
{
public:
    GridRender();
    ~GridRender() = default;

    void render() const;

private:
    ly::ref<ly::renderer::VertexArray> m_gridVao;
    ly::ref<ly::renderer::Shader> m_gridShader;
};
