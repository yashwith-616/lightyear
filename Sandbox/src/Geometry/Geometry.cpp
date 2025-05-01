#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::s_Geometry = nullptr;
static ly::Ref<ly::renderer::VertexArray> Init(float* vertexData,
                                               uint32_t vertexSize,
                                               uint32_t* indexData,
                                               uint32_t indexSize) {
    ly::renderer::BufferElement bufferElement(
        ly::renderer::ShaderDataType::Float3, "Positions", false);
    ly::renderer::BufferLayout bufferLayout = { bufferElement };

    auto* vertexBuffer = ly::renderer::VertexBuffer::Create(vertexData, vertexSize);
    vertexBuffer->SetLayout(bufferLayout);

    auto* indexBuffer = ly::renderer::IndexBuffer::Create(indexData, indexSize);

    auto* vertexArray = ly::renderer::VertexArray::Create();
    vertexArray->AddVertexBuffer(ly::Ref<ly::renderer::VertexBuffer>(vertexBuffer));
    vertexArray->SetIndexBuffer(indexBuffer);

    return ly::Ref<ly::renderer::VertexArray>(vertexArray);
}

static Geometry* GetGeomtry() {
    if (!Geometry::s_Geometry) {
        return new Geometry();
    }

    return Geometry::s_Geometry;
}

Geometry::Geometry() {
    m_PlaneVertexArray = Init(g_PlaneVertices.data()->data(),
                              g_PlaneVertexCount,
                              g_PlaneIndices.data(),
                              g_PlaneIndexCount);

    m_CubeVertexArray = Init(const_cast<float*>(g_CubeVertices),
                             g_CubeVertexCount,
                             const_cast<uint32_t*>(g_CubeIndices),
                             g_CubeIndexCount);

    // Not yet intialized
    m_SphereVertexArray   = Init(nullptr, 0, nullptr, 0);
    m_CylinderVertexArray = Init(nullptr, 0, nullptr, 0);
    m_TeapotVertexArray   = Init(nullptr, 0, nullptr, 0);

    s_Geometry = this;
}

void Geometry::DrawPlane(ly::renderer::RendererAPI* renderAPI) {
    renderAPI->DrawIndexed(ly::Ref<ly::renderer::VertexArray>(GetGeomtry()->m_PlaneVertexArray));
}

void Geometry::DrawCube(ly::renderer::RendererAPI* renderAPI) {
    renderAPI->DrawIndexed(ly::Ref<ly::renderer::VertexArray>(GetGeomtry()->m_CubeVertexArray));
}

void Geometry::DrawSphere(ly::renderer::RendererAPI* renderAPI) {}

void Geometry::DrawCylinder(ly::renderer::RendererAPI* renderAPI) {}

void Geometry::DrawTeapot(ly::renderer::RendererAPI* renderAPI) {}
