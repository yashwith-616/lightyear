#pragma once

#include "Lightyear/LightyearCore.h"
#include "Lightyear/Renderer/Primitives/RenderTypes.h"

namespace ly::renderer
{

/**
 * @brief Represents a single attribute inside a vertex buffer layout
 *
 * Stores metadata like name, shader type, size, offset, and normalization state,
 * which is necessary for correctly interpreting vertex data in GPU shaders
 */
struct LIGHTYEAR_API BufferElement
{
    std::string name{};
    uint32_t size{0};
    uint32_t offset{0};
    ShaderDataType type{ShaderDataType::NONE};
    bool isNormalized{true};

    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string name, bool isNormalized) :
        name(std::move(name)), size(getShaderDataTypeSize(type)), type(type), isNormalized(isNormalized)
    {}

    /**
     * @brief Returns how many components make up this attribute (e.g., Float3 = (x, y, z)).
     * @return Number of components for this ShaderDataType.
     */
    [[nodiscard]] uint32_t getComponentCount() const
    {
        switch (type)
        {
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
class LIGHTYEAR_API BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    [[nodiscard]] uint32_t GetStride() const { return m_stride; }
    [[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_elements; }

    // NOLINTBEGIN(*-identifier-naming)
    [[nodiscard]] std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::iterator end() { return m_elements.end(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
    // NOLINTEND(*-identifier-naming)

protected:
    void calculateOffsetsAndStride();

private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;
};

/**
 * @brief Manages vertex buffer attributes and their memory layout within a buffer.
 */
class LIGHTYEAR_API VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    static Ref<VertexBuffer> create(uint32_t sizeInBytes);
    static Ref<VertexBuffer> create(std::span<const float> vertices);

    virtual void bind() const = 0;
    virtual void unBind() const = 0;

    virtual void setData(const void* data, uint32_t size) = 0;

    [[nodiscard]] virtual const BufferLayout& getLayout() const = 0;
    virtual void setLayout(const BufferLayout& layout) = 0;
};

/**
 * @brief Holds indices that define the order in which vertices are drawn.
 */
class LIGHTYEAR_API IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    static Ref<IndexBuffer> create(std::span<const uint32_t> indices);

    virtual void bind() const = 0;
    virtual void unBind() const = 0;

    [[nodiscard]] virtual uint32_t getCount() const = 0;
};

} // namespace ly::renderer
