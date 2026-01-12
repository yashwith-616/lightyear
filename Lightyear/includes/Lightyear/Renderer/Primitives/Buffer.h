#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer {

/**
 * @brief Represents a single attribute inside a vertex buffer layout
 *
 * Stores metadata like name, shader type, size, offset, and normalization state,
 * which is necessary for correctly interpreting vertex data in GPU shaders
 */
struct LIGHTYEAR_API BufferElement {
    std::string Name{};
    uint32_t Size{ 0 };
    uint32_t Offset{ 0 };
    ShaderDataType Type{ ShaderDataType::NONE };
    bool IsNormalized{ true };

    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string name, bool isNormalized)
        : Name(std::move(name)), Size(GetShaderDataTypeSize(type)), Type(type), IsNormalized(isNormalized) {}

    /**
     * @brief Returns how many components make up this attribute (e.g., Float3 = (x, y, z)).
     * @return Number of components for this ShaderDataType.
     */
    [[nodiscard]] uint32_t GetComponentCount() const {
        switch (Type) {
            case ShaderDataType::BOOL:
            case ShaderDataType::FLOAT:
            case ShaderDataType::INT:
                return 1;
            case ShaderDataType::FLOAT2:
            case ShaderDataType::INT2:
                return 2;
            case ShaderDataType::FLOAT3:
            case ShaderDataType::INT3:
            case ShaderDataType::MAT3:
                return 3;
            case ShaderDataType::FLOAT4:
            case ShaderDataType::INT4:
            case ShaderDataType::MAT4:
                return 4;
            case ShaderDataType::NONE:
            default:
                LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
                return -1;
        }

        LY_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return -1;
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
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    [[nodiscard]] uint32_t GetStride() const { return m_Stride; }
    [[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_Elements; }

    // NOLINTBEGIN(*-identifier-naming)
    [[nodiscard]] std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
    // NOLINTEND(*-identifier-naming)

protected:
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
    virtual ~VertexBuffer() = default;

    static Ref<VertexBuffer> Create(uint32_t sizeInBytes);
    static Ref<VertexBuffer> Create(std::span<const float> vertices);

    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    [[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout)          = 0;
};

/**
 * @brief Holds indices that define the order in which vertices are drawn.
 */
class LIGHTYEAR_API IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    static Ref<IndexBuffer> Create(std::span<const uint32_t> indices);

    virtual void Bind() const   = 0;
    virtual void UnBind() const = 0;

    [[nodiscard]] virtual uint32_t GetCount() const = 0;
};

}  // namespace ly::renderer