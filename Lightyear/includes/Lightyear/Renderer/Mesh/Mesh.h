#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::renderer {

class LIGHTYEAR_API Mesh {
public:
    explicit Mesh(Ref<VertexArray> vertexArray) : m_VertexArray(std::move(vertexArray)) {}

    // TODO: Require API to modify each of these

private:
    Ref<VertexArray> m_VertexArray;
    // TODO: Add material here
};

class LIGHTYEAR_API MeshObject {
public:
    explicit MeshObject(Ref<Mesh> mesh) : m_Mesh(std::move(mesh)) {}

    // TODO: Add API to update local transform

private:
    Ref<Mesh> m_Mesh;
    glm::mat4 m_LocalTransform{};
};

class LIGHTYEAR_API MeshAsset {
public:
    explicit MeshAsset(std::vector<Ref<MeshObject>> meshObjects) : m_MeshObjects(std::move(meshObjects)) {}

    // TODO: Add APIs to add or remove mesh object

private:
    std::vector<Ref<MeshObject>> m_MeshObjects;
};

}  // namespace ly::renderer
