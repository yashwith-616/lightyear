/**
 * @file Buffer.h
 * @author Yashwith
 * @brief
 * @date 2026-03-01
 */
#pragma once

#include <cstdint>
#include <span>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace ly::renderer
{
class LogicalDevice;

enum class VertexAttributeType
{
    Float,
    Float2,
    Float3,
    Float4, // Standard floats
    Byte4,
    Byte4N, // Colors (N = Normalized)
    Short2,
    Short2N,  // UVs or Normals
    Int10Pack // Packed Normals (10-10-10-2)
};


enum class VertexAttributeSlot : uint8_t
{
    Position = 0,
    Uv,
    Color,
    Normal,
    Tangent,
    BiTangent,
    TexCoord,
    Max
};

/*
 * A buffer element defines all attributes of a vertex.
 * Each attribute information is stored as the size taken up by individual data.
 */
struct BufferElement
{
    uint32_t offset{0};
    VertexAttributeSlot slot;
    VertexAttributeType type;
    bool isNormalized; // useful to know if the value of the data is in range [0.0-1.0]

    explicit BufferElement(VertexAttributeSlot slot, VertexAttributeType type, bool isNormalized);
};

/**
 * Holds information of all the attributes for a vertex data
 */
class BufferLayout
{
public:
    explicit BufferLayout() = default;
    explicit BufferLayout(std::vector<BufferElement> elements);

    BufferLayout(BufferLayout const&) = delete;
    BufferLayout(BufferLayout&&) = default;

    BufferLayout& operator=(BufferLayout const&) = delete;
    BufferLayout& operator=(BufferLayout&&) = default;

    void addElement(BufferElement element);

    uint32_t getStride() const { return m_stride; }

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions(uint32_t binding) const;

private:
    void calculateOffsetsAndStride();
    void updateStride();

    std::vector<BufferElement> m_elements;
    uint32_t m_stride{0};
};

class VertexBuffer
{
public:
    /**
     * Vulkan vertex buffer
     *
     * \param layout the buffer layout reference. Once a buffer is created it can't be repurposed
     * \param data The data is not owned by the buffer, used for one-time update to GPU
     */
    explicit VertexBuffer(LogicalDevice const& device, BufferLayout const& layout, std::span<std::byte const> data);

    VertexBuffer(VertexBuffer const&) = delete;
    VertexBuffer(VertexBuffer&&) = default;

    VertexBuffer& operator=(VertexBuffer const&) = delete;

    BufferLayout const& getLayout() const { return m_layout; }

    vk::VertexInputBindingDescription getBindingDescription(uint32_t binding) const;

    vk::raii::Buffer const& getHandle() const { return m_bufferHandle; }

private:
    BufferLayout const& m_layout;
    vk::raii::Buffer m_bufferHandle{nullptr};
    vk::raii::DeviceMemory m_bufferMemoryHandle{nullptr};
};

class IndexBuffer
{
public:
    explicit IndexBuffer(LogicalDevice const& device, std::span<std::byte const> data);

    IndexBuffer(IndexBuffer const&) = delete;
    IndexBuffer(IndexBuffer&&) = default;

    IndexBuffer& operator=(IndexBuffer const&) = delete;

    vk::raii::Buffer const& getHandle() const { return m_bufferHandle; }

private:
    vk::raii::Buffer m_bufferHandle{nullptr};
    vk::raii::DeviceMemory m_bufferMemoryHandle{nullptr};
};

} // namespace ly::renderer
