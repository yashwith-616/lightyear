#include "Sandbox/Helpers/GridRender.h"

namespace
{
// NOLINTNEXTLINE
constexpr float quadVertices[] = {
    -1.0f,
    -1.0f, // 0: bottom left
    1.0f,
    -1.0f, // 1: bottom right
    1.0f,
    1.0f, // 2: top right
    -1.0f,
    1.0f // 3: top left
};

// NOLINTNEXTLINE
constexpr uint32_t quadIndices[] = {0, 1, 2, 2, 3, 0};
} // namespace

constexpr std::span<const float> g_QuadVertexSpan = quadVertices;
constexpr std::span<const uint32_t> g_QuadIndexSpan = quadIndices;

GridRender::GridRender()
{
    const ly::renderer::BufferElement bufferElement(ly::renderer::ShaderDataType::FLOAT2, "Positions", false);
    const ly::renderer::BufferLayout bufferLayout = {bufferElement};

    const auto vertexBuffer = ly::renderer::VertexBuffer::create(g_QuadVertexSpan);
    vertexBuffer->setLayout(bufferLayout);

    const auto indexBuffer = ly::renderer::IndexBuffer::create(g_QuadIndexSpan);

    m_GridVAO = ly::renderer::VertexArray::create();
    m_GridVAO->addVertexBuffer(vertexBuffer);
    m_GridVAO->setIndexBuffer(indexBuffer);

    m_GridShader = ly::renderer::Shader::Create("GridShader", g_GridShader);
}

void GridRender::Render() const
{
    ly::renderer::Renderer::Submit(ly::renderer::RenderSubmission(m_GridShader, m_GridVAO, nullptr, glm::mat4(1.f)));
}
