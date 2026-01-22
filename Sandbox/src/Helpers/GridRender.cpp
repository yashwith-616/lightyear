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

constexpr std::span<float const> k_quadVertexSpan = quadVertices;
constexpr std::span<uint32_t const> k_quadIndexSpan = quadIndices;

GridRender::GridRender()
{
    ly::renderer::BufferElement const bufferElement(ly::renderer::ShaderDataType::Float2, "Positions", false);
    ly::renderer::BufferLayout const bufferLayout = {bufferElement};

    auto const vertexBuffer = ly::renderer::VertexBuffer::create(k_quadVertexSpan);
    vertexBuffer->setLayout(bufferLayout);

    auto const indexBuffer = ly::renderer::IndexBuffer::create(k_quadIndexSpan);

    m_gridVao = ly::renderer::VertexArray::create();
    m_gridVao->addVertexBuffer(vertexBuffer);
    m_gridVao->setIndexBuffer(indexBuffer);

    m_gridShader = ly::renderer::Shader::create("GridShader", k_gridShader);
}

void GridRender::render() const
{
    ly::renderer::Renderer::submit(ly::renderer::RenderSubmission(m_gridShader, m_gridVao, nullptr, glm::mat4(1.f)));
}
