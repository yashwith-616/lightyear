#pragma once

#include "Lightyear/Common/Assertions.h"
#include "Lightyear/pch/lypch.h"

namespace ly::renderer
{

enum class ShaderType : uint8_t
{
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

enum class ShaderDataType : uint8_t
{
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
    Bool,
    Max
};

enum class UniformBufferBlockBinding : uint8_t
{
    None = 0,
    Camera,
    Scene,
    Material,
    Object,
    Max
};

enum class VertexAttributeType : uint8_t
{
    None = 0,
    Position,
    Normal,
    TexCoord,
    Max
};

// === Utility functions ===
inline std::string_view getVertexAttribute(VertexAttributeType const type)
{
    switch (type)
    {
    case VertexAttributeType::Position:
        return "Position";
    case VertexAttributeType::Normal:
        return "Normal";
    case VertexAttributeType::TexCoord:
        return "TexCoord";
    case VertexAttributeType::None:
    default:
        {
            LY_CORE_ASSERT(false, "Vertex attribute type not supported");
            return "None";
        }
    }
}

inline uint32_t getShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Bool:
        return 1;
    case ShaderDataType::Float:
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Float2:
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Float3:
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Float4:
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::None:
    default:
        LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return -1;
    }
};

// === Alias for different Shader types ===
// TODO: Move to a templated struct for better compile time safety. Need to be removed
using textureHandle = uint32_t;
using vertexBufferHandle = uint32_t;
using indexBufferHandle = uint32_t;
using vertexArrayHandle = uint32_t;

using shaderHandle = uint32_t;
using framebufferHandle = uint32_t;
using uniformBufferHandle = uint32_t;
using renderbufferHandle = uint32_t;

using pipelineHandle = uint32_t;
using materialHandle = uint32_t;
using samplerHandle = uint32_t;
using computeShaderHandle = uint32_t;

constexpr size_t k_shaderTypeCount = static_cast<size_t>(ShaderType::Max);

} // namespace ly::renderer
