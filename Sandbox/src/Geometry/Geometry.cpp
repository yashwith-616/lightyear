#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::s_Geometry = nullptr;

namespace {
ly::Ref<ly::renderer::VertexArray> Init(std::span<const float> vertexData, std::span<const uint32_t> indexData) {
    const ly::renderer::BufferElement bufferElement(ly::renderer::ShaderDataType::FLOAT3, "Positions", false);
    const ly::renderer::BufferElement texCoordBuffer(ly::renderer::ShaderDataType::FLOAT2, "TexCoord", false);
    const ly::renderer::BufferLayout bufferLayout = { bufferElement, texCoordBuffer };

    const auto vertexBuffer = ly::renderer::VertexBuffer::Create(vertexData);
    vertexBuffer->SetLayout(bufferLayout);

    const auto indexBuffer = ly::renderer::IndexBuffer::Create(indexData);

    auto vertexArray = ly::renderer::VertexArray::Create();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    return vertexArray;
}
}  // namespace

static Geometry* GetGeometry() {
    if (Geometry::s_Geometry == nullptr) {
        return new Geometry();
    }

    return Geometry::s_Geometry;
}

Geometry::Geometry() {
    CubeVertexArray  = Init(g_CubeVerticesSpan, g_CubeIndicesSpan);
    PlaneVertexArray = Init(g_PlaneVerticesSpan, g_PlaneIndicesSpan);

    s_Geometry = this;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetPlane() {
    return GetGeometry()->PlaneVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCube() {
    return GetGeometry()->CubeVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetCylinder() {
    return GetGeometry()->CylinderVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetSphere() {
    return GetGeometry()->SphereVertexArray;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetTeapot() {
    return GetGeometry()->TeapotVertexArray;
}
