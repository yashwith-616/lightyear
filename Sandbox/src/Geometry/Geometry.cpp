#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::s_Geometry = nullptr;

namespace
{

ly::Ref<ly::renderer::VertexArray> Init(std::span<const float> vertexData, std::span<const uint32_t> indexData)
{
    const ly::renderer::BufferElement bufferElement(ly::renderer::ShaderDataType::FLOAT3, "aPosition", false);
    const ly::renderer::BufferElement texCoordBuffer(ly::renderer::ShaderDataType::FLOAT2, "aTexCoord", false);
    const ly::renderer::BufferLayout bufferLayout = {bufferElement, texCoordBuffer};

    const auto vertexBuffer = ly::renderer::VertexBuffer::create(vertexData);
    vertexBuffer->setLayout(bufferLayout);

    const auto indexBuffer = ly::renderer::IndexBuffer::create(indexData);

    auto vertexArray = ly::renderer::VertexArray::create();
    vertexArray->addVertexBuffer(vertexBuffer);
    vertexArray->setIndexBuffer(indexBuffer);

    return vertexArray;
}

} // namespace

static Geometry* GetGeometry()
{
    if (Geometry::s_Geometry == nullptr)
    {
        Geometry::s_Geometry = new Geometry;
    }

    return Geometry::s_Geometry;
}

Geometry::Geometry()
{
    CubeVertexArray = Init(g_CubeVerticesSpan, g_CubeIndicesSpan);
    PlaneVertexArray = Init(g_PlaneVerticesSpan, g_PlaneIndicesSpan);

    s_Geometry = this;
}

ly::Ref<ly::renderer::VertexArray> Geometry::GetPlane() { return GetGeometry()->PlaneVertexArray; }

ly::Ref<ly::renderer::VertexArray> Geometry::GetCube() { return GetGeometry()->CubeVertexArray; }

ly::Ref<ly::renderer::VertexArray> Geometry::GetCylinder() { return GetGeometry()->CylinderVertexArray; }

ly::Ref<ly::renderer::VertexArray> Geometry::GetSphere() { return GetGeometry()->SphereVertexArray; }

ly::Ref<ly::renderer::VertexArray> Geometry::GetTeapot() { return GetGeometry()->TeapotVertexArray; }
