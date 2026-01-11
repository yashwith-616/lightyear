#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer
{

class LIGHTYEAR_API Mesh
{
public:
    explicit Mesh(Ref<VertexArray> vertexArray) : m_vertexArray(std::move(vertexArray)) {}

    // TODO: Require API to modify each of these

private:
    Ref<VertexArray> m_vertexArray;
    // TODO: Add material here
};

class LIGHTYEAR_API MeshObject
{
public:
    explicit MeshObject(Ref<Mesh> mesh) : m_mesh(std::move(mesh)) {}

    // TODO: Add API to update local transform

private:
    Ref<Mesh> m_mesh;
    glm::mat4 m_localTransform{};
};

class LIGHTYEAR_API MeshAsset
{
public:
    explicit MeshAsset(std::vector<Ref<MeshObject>> meshObjects) : m_meshObjects(std::move(meshObjects)) {}

    // TODO: Add APIs to add or remove mesh object

private:
    std::vector<Ref<MeshObject>> m_meshObjects;
};

} // namespace ly::renderer
