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
    std::string name{};
    uint32_t size{ 0 };
    uint32_t offset{ 0 };
    ShaderDataType type{ ShaderDataType::None };
    bool isNormalized{ true };

    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string name, bool isNormalized)
        : name(std::move(name)), size(getShaderDataTypeSize(type)), type(type), isNormalized(isNormalized) {}

    /**
     * @brief Returns how many components make up this attribute (e.g., Float3 = (x, y, z)).
     * @return Number of components for this ShaderDataType.
     */
    [[nodiscard]] uint32_t getComponentCount() const {
        switch (type) {
            case ShaderDataType::Bool:
            case ShaderDataType::Float:
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Float2:
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Float3:
            case ShaderDataType::Int3:
            case ShaderDataType::Mat3:
                return 3;
            case ShaderDataType::Float4:
            case ShaderDataType::Int4:
            case ShaderDataType::Mat4:
                return 4;
            case ShaderDataType::None:
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

    [[nodiscard]] uint32_t getStride() const { return m_stride; }
    [[nodiscard]] std::vector<BufferElement> const& getElements() const { return m_elements; }

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
class LIGHTYEAR_API VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    static ref<VertexBuffer> create(uint32_t sizeInBytes);
    static ref<VertexBuffer> create(std::span<float const> vertices);

    virtual void bind() const;
    virtual void unBind() const;

    virtual void setData(void const* data, uint32_t size) = 0;

    [[nodiscard]] virtual BufferLayout const& getLayout() const = 0;
    virtual void setLayout(BufferLayout const& layout)          = 0;
};

/**
 * @brief Holds indices that define the order in which vertices are drawn.
 */
class LIGHTYEAR_API IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    static ref<IndexBuffer> create(std::span<uint32_t const> indices);

    virtual void bind() const   = 0;
    virtual void unBind() const = 0;

    [[nodiscard]] virtual uint32_t getCount() const = 0;
};

}  // namespace ly::renderer