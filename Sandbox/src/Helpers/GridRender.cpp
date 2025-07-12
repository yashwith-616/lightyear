#include "Sandbox/Helpers/GridRender.h"
#include "Sandbox/Core/Camera/EditorCamera.h"

constexpr float quadVertices[] = {
    -1.0f, -1.0f,  // 0: bottom left
    1.0f,  -1.0f,  // 1: bottom right
    1.0f,  1.0f,   // 2: top right
    -1.0f, 1.0f    // 3: top left
};

constexpr uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };

const std::unordered_map<ly::renderer::ShaderType, ly::CPath> g_GridShader = {
    { ly::renderer::ShaderType::VERTEX, ASSET_DIR "/Shaders/Vertex/S_Grid.vert" },
    { ly::renderer::ShaderType::FRAGMENT, ASSET_DIR "/Shaders/Fragment/S_Grid.frag" }
};

GridRender::GridRender() {
    ly::renderer::BufferElement bufferElement(ly::renderer::ShaderDataType::FLOAT2, "Positions", false);
    ly::renderer::BufferLayout bufferLayout = { bufferElement };

    auto vertexBuffer = ly::renderer::VertexBuffer::Create(quadVertices, sizeof(quadVertices));
    vertexBuffer->SetLayout(bufferLayout);

    auto indexBuffer = ly::renderer::IndexBuffer::Create(const_cast<uint32_t*>(quadIndices), sizeof(quadIndices));

    m_GridVAO = ly::renderer::VertexArray::Create();
    m_GridVAO->AddVertexBuffer(vertexBuffer);
    m_GridVAO->SetIndexBuffer(indexBuffer);

    m_GridShader = ly::renderer::Shader::Create("GridShader", g_GridShader);
}

void GridRender::Render() {
    ly::renderer::Renderer::Submit(ly::renderer::RenderSubmission(m_GridShader, m_GridVAO, nullptr, glm::mat4(1.f)));
}