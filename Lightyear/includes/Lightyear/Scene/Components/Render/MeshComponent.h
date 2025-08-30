#pragma once

#include "Lightyear/LightyearCore.h"

#include "Lightyear/Renderer/Primitives/Shader.h"
#include "Lightyear/Renderer/Primitives/Texture.h"
#include "Lightyear/Renderer/Primitives/VertexArray.h"

namespace ly::scene {

struct MeshComponent {
    Ref<renderer::VertexArray> MeshAsset;
    Ref<renderer::Shader> ShaderAsset;
    Ref<renderer::Texture> TextureAsset;

    MeshComponent(Ref<renderer::VertexArray> vertexArray, Ref<renderer::Shader> shader, Ref<renderer::Texture> texture)
        : MeshAsset(std::move(vertexArray)), ShaderAsset(std::move(shader)), TextureAsset(std::move(texture)) {}

    MeshComponent(const MeshComponent&)            = default;
    MeshComponent(MeshComponent&&)                 = default;
    MeshComponent& operator=(const MeshComponent&) = default;
    MeshComponent& operator=(MeshComponent&&)      = default;
};

}  // namespace ly::scene