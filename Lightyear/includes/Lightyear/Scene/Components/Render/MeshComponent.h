#pragma once

#include "Lightyear/LightyearCore.h"

#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::scene
{

struct MeshComponent
{
    ref<renderer::VertexArray> meshAsset;
    ref<renderer::Shader> shaderAsset;
    ref<renderer::Texture> textureAsset;

    MeshComponent() = default;
    MeshComponent(
        ref<renderer::VertexArray> vertexArray, ref<renderer::Shader> shader, ref<renderer::Texture> texture) :
        meshAsset(std::move(vertexArray)), shaderAsset(std::move(shader)), textureAsset(std::move(texture))
    {}

    MeshComponent(MeshComponent const&) = default;
    MeshComponent(MeshComponent&&) = default;
    MeshComponent& operator=(MeshComponent const&) = default;
    MeshComponent& operator=(MeshComponent&&) = default;
};

} // namespace ly::scene
