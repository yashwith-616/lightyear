#pragma once

#include <lypch.h>

namespace ly::renderer {

enum class ShaderType : uint8_t {
    None = 0,
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessControl,
    TessEvaluation,
    Pixel,
    Max
};

enum class ShaderDataType : uint8_t {
    None = 0,

    Float,
    Float2,
    Float3,
    Float4,

    Mat3,
    Mat4,

    Int,
    Int2,
    Int3,
    Int4,

    Bool
};

enum class VertexAttributeType : uint8_t { None = 0, Position, Normal, TexCoord };

using TextureHandle      = uint32_t;
using VertexBufferHandle = uint32_t;
using IndexBufferHandle  = uint32_t;
using VertexArrayHandle  = uint32_t;

using ShaderHandle        = uint32_t;
using FramebufferHandle   = uint32_t;
using UniformBufferHandle = uint32_t;
using RenderbufferHandle  = uint32_t;

using PipelineHandle      = uint32_t;
using MaterialHandle      = uint32_t;
using SamplerHandle       = uint32_t;
using ComputeShaderHandle = uint32_t;

constexpr size_t ShaderTypeCount = static_cast<size_t>(ShaderType::Max);

}  // namespace ly::renderer