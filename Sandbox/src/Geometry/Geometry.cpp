#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::s_Geometry = nullptr;
static ly::Ref<ly::renderer::VertexArray> Init(const float* vertexData,
                                               uint32_t vertexSize,
                                               uint32_t* indexData,
                                               uint32_t indexSize) {
    const ly::renderer::BufferElement bufferElement(ly::renderer::ShaderDataType::FLOAT3, "Positions", false);
    const ly::renderer::BufferElement texCoordBuffer(ly::renderer::ShaderDataType::FLOAT2, "TexCoord", false);
    const ly::renderer::BufferLayout bufferLayout = { bufferElement, texCoordBuffer };

    auto vertexBuffer = ly::renderer::VertexBuffer::Create(vertexData, vertexSize);
    vertexBuffer->SetLayout(bufferLayout);

    const auto indexBuffer = ly::renderer::IndexBuffer::Create(indexData, indexSize);

    auto vertexArray = ly::renderer::VertexArray::Create();
    vertexArray->AddVertexBuffer(ly::Ref(vertexBuffer));
    vertexArray->SetIndexBuffer(indexBuffer);

    return ly::Ref(vertexArray);
}

static Geometry* GetGeometry() {
    if (!Geometry::s_Geometry) {
        return new Geometry();
    }

    return Geometry::s_Geometry;
}

Geometry::Geometry() {
    m_PlaneVertexArray =
        Init(g_PlaneVertices.data()->data(), g_PlaneVertexCount, g_PlaneIndices.data(), g_PlaneIndexCount);

    m_CubeVertexArray = Init(
        g_CubeVertices.data()->data(), sizeof(g_CubeVertices), const_cast<uint32_t*>(g_CubeIndices), g_CubeIndexCount);

    // Not yet initialized
    m_SphereVertexArray   = Init(nullptr, 0, nullptr, 0);
    m_CylinderVertexArray = Init(nullptr, 0, nullptr, 0);
    m_TeapotVertexArray   = Init(nullptr, 0, nullptr, 0);

    s_Geometry = this;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetPlane() {
    return GetGeometry()->m_PlaneVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCube() {
    return GetGeometry()->m_CubeVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCylinder() {
    return GetGeometry()->m_CylinderVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetSphere() {
    return GetGeometry()->m_SphereVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetTeapot() {
    return GetGeometry()->m_TeapotVertexArray;
}
