#include "Sandbox/Geometry/Geometry.h"
#include "Sandbox/Geometry/Shapes/ShapeVertex.h"

Geometry* Geometry::geometry = nullptr;

namespace
{

ly::ref<ly::renderer::VertexArray> init(std::span<float const> vertexData, std::span<uint32_t const> indexData)
{
    ly::renderer::BufferElement const bufferElement(ly::renderer::ShaderDataType::Float3, "aPosition", false);
    ly::renderer::BufferElement const texCoordBuffer(ly::renderer::ShaderDataType::Float2, "aTexCoord", false);
    ly::renderer::BufferLayout const bufferLayout = {bufferElement, texCoordBuffer};

    auto const vertexBuffer = ly::renderer::VertexBuffer::create(vertexData);
    vertexBuffer->setLayout(bufferLayout);

    auto const indexBuffer = ly::renderer::IndexBuffer::create(indexData);

    auto vertexArray = ly::renderer::VertexArray::create();
    vertexArray->addVertexBuffer(vertexBuffer);
    vertexArray->setIndexBuffer(indexBuffer);

    return vertexArray;
}

} // namespace

static Geometry* getGeometry()
{
    if (Geometry::geometry == nullptr)
    {
        Geometry::geometry = new Geometry;
    }

    return Geometry::geometry;
}

Geometry::Geometry()
{
    cubeVertexArray = init(k_cubeVerticesSpan, k_cubeIndicesSpan);
    planeVertexArray = init(k_planeVerticesSpan, k_planeIndicesSpan);

    geometry = this;
}

ly::ref<ly::renderer::VertexArray> Geometry::getPlane() { return getGeometry()->planeVertexArray; }

ly::ref<ly::renderer::VertexArray> Geometry::getCube() { return getGeometry()->cubeVertexArray; }

ly::ref<ly::renderer::VertexArray> Geometry::getSphere() { return getGeometry()->sphereVertexArray; }

ly::ref<ly::renderer::VertexArray> Geometry::getCylinder() { return getGeometry()->cylinderVertexArray; }

ly::ref<ly::renderer::VertexArray> Geometry::getTeapot() { return getGeometry()->teapotVertexArray; }
