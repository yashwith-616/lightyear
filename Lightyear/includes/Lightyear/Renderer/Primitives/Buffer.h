#pragma once

#include "Lightyear/LightyearCore.h"
#include "RenderTypes.h"

namespace ly::renderer {

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
    }

    LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
};

/**
 * @brief Represents a single attribute inside a vertex buffer layout
 *
 * Stores metadata like name, shader type, size, offset, and normalization state,
 * which is necessary for correctly interpreting vertex data in GPU shaders
 */
struct LIGHTYEAR_API BufferElement {
    std::string_view Name{};
    uint32_t Size{ 0 };
    uint32_t Offset{ 0 };
    ShaderDataType Type{ ShaderDataType::None };
    bool IsNormalized{ true };

    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string_view name, bool isNormalized)
        : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
          IsNormalized(isNormalized) {}

    /**
     * @brief Returns how many components make up this attribute (e.g., Float3 = 3 components).
     * @return Number of components for this ShaderDataType.
     */
    uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Mat3:
                return 3;  // 3* float3
            case ShaderDataType::Mat4:
                return 4;  // 4* float4
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Bool:
                return 1;
        }

        LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
};

/**
 * @brief Describes the layout of elements within a vertex buffer
 *
 * BufferLayout determines the structure of a vertex by specifying the sequence of attributes
 * and how they are packed
 *
 * It stores list of BufferElements, calculates offsets for each attribute, and determines the
 * stride (the total size of the vertex)
 */
class LIGHTYEAR_API BufferLayout {
public:
    BufferLayout() {}

    BufferLayout(std::initializer_list<BufferElement> elements);

    uint32_t GetStride() const { return m_Stride; }
    const std::vector<BufferElement>& GetElements() const { return m_Elements; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
    void CalculateOffsetsAndStride();

private:
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

/**
 * @brief Manages vertex buffer attributes and their memory layout within a buffer.
 */
class LIGHTYEAR_API VertexBuffer {
public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& GetLayout() const      = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static VertexBuffer* Create(uint32_t size);
    static VertexBuffer* Create(float* vertices, uint32_t size);
};

/**
 * @brief Holds indices that define the order in which vertices are drawn.
 */
class LIGHTYEAR_API IndexBuffer {
public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t size);
};

}  // namespace ly::renderer