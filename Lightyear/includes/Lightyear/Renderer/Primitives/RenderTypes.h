#pragma once

#include "Lightyear/Common/Assertions.h"
#include "Lightyear/pch/lypch.h"

namespace ly::renderer
{

enum class ShaderType : uint8_t
{
    NONE = 0,
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    COMPUTE,
    TESS_CONTROL,
    TESS_EVALUATION,
    PIXEL,
    MAX
};

enum class ShaderDataType : uint8_t
{
    NONE = 0,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MAT3,
    MAT4,
    INT,
    INT2,
    INT3,
    INT4,
    BOOL,
    MAX
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
inline std::string_view getVertexAttribute(const VertexAttributeType type)
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

inline uint32_t getShaderDataTypeSize(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::BOOL:
        return 1;
    case ShaderDataType::FLOAT:
    case ShaderDataType::INT:
        return 4;
    case ShaderDataType::FLOAT2:
    case ShaderDataType::INT2:
        return 4 * 2;
    case ShaderDataType::FLOAT3:
    case ShaderDataType::INT3:
        return 4 * 3;
    case ShaderDataType::FLOAT4:
    case ShaderDataType::INT4:
        return 4 * 4;
    case ShaderDataType::MAT3:
        return 4 * 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4 * 4;
    case ShaderDataType::NONE:
    default:
        LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return -1;
    }
}

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

constexpr size_t k_shaderTypeCount = static_cast<size_t>(ShaderType::MAX);

} // namespace ly::renderer
