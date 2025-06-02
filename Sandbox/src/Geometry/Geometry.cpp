#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::s_Geometry = nullptr;
static ly::Ref<ly::renderer::VertexArray> Init(const float* vertexData,
                                               uint32_t vertexSize,
                                               uint32_t* indexData,
                                               uint32_t indexSize) {
    ly::renderer::BufferElement bufferElement(
        ly::renderer::ShaderDataType::Float3, "Positions", false);
    ly::renderer::BufferElement texCoordBuffer(
        ly::renderer::ShaderDataType::Float2, "TexCoord", false);
    ly::renderer::BufferLayout bufferLayout = { bufferElement, texCoordBuffer };

    auto vertexBuffer = ly::renderer::VertexBuffer::Create(vertexData, vertexSize);
    vertexBuffer->SetLayout(bufferLayout);

    auto indexBuffer = ly::renderer::IndexBuffer::Create(indexData, indexSize);

    auto vertexArray = ly::renderer::VertexArray::Create();
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

    m_CubeVertexArray = Init(
        g_CubeVertices.data()->data(),
        static_cast<uint32_t>(g_CubeVertices.size() * g_CubeVertices[0].size() * sizeof(float)),
        const_cast<uint32_t*>(g_CubeIndices),
        static_cast<uint32_t>(g_CubeIndexCount * sizeof(uint32_t)));

    // Not yet intialized
    m_SphereVertexArray   = Init(nullptr, 0, nullptr, 0);
    m_CylinderVertexArray = Init(nullptr, 0, nullptr, 0);
    m_TeapotVertexArray   = Init(nullptr, 0, nullptr, 0);

    s_Geometry = this;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetPlane() {
    return GetGeomtry()->m_PlaneVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCube() {
    return GetGeomtry()->m_CubeVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCylinder() {
    return GetGeomtry()->m_CylinderVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetSphere() {
    return GetGeomtry()->m_SphereVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetTeapot() {
    return GetGeomtry()->m_TeapotVertexArray;
}
