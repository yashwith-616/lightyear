#pragma once

#include "Lightyear/Common/Assertions.h"
#include "Lightyear/pch/lypch.h"

namespace ly::renderer {

enum class ShaderType : uint8_t {
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

enum class ShaderDataType : uint8_t {
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

enum class UniformBufferBlockBinding : uint8_t { NONE = 0, CAMERA, SCENE, MATERIAL, OBJECT, MAX };

enum class VertexAttributeType : uint8_t { NONE = 0, POSITION, NORMAL, TEX_COORD, MAX };

// === Utility functions ===
inline std::string_view GetVertexAttribute(const VertexAttributeType type) {
    switch (type) {
        case VertexAttributeType::POSITION:
            return "Position";
        case VertexAttributeType::NORMAL:
            return "Normal";
        case VertexAttributeType::TEX_COORD:
            return "TexCoord";
        case VertexAttributeType::NONE:
        default: {
            LY_CORE_ASSERT(false, "Vertex attribute type not supported");
            return "None";
        }
    }
}

inline uint32_t GetShaderDataTypeSize(const ShaderDataType type) {
    switch (type) {
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
};

// === Alias for different Shader types ===
// TODO: Move to a templated struct for better compile time safety. Need to be removed
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

constexpr size_t g_ShaderTypeCount = static_cast<size_t>(ShaderType::MAX);

}  // namespace ly::renderer