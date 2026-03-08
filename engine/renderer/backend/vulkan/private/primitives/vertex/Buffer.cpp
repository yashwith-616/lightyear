/**
 * @file Buffer.cpp
 * @author Yashwith
 * @brief
 * @date 2026-03-01
 */
#include "Buffer.h"

#include "device/LogicalDevice.h"

namespace
{
vk::Format getVulkanFormat(ly::renderer::VertexAttributeType type, bool isNormalized)
{
    using ly::renderer::VertexAttributeType;
    switch (type)
    {
    case VertexAttributeType::Float:
        return vk::Format::eR32Sfloat;
    case VertexAttributeType::Float2:
        return vk::Format::eR32G32Sfloat;
    case VertexAttributeType::Float3:
        return vk::Format::eR32G32B32Sfloat;
    case VertexAttributeType::Float4:
        return vk::Format::eR32G32B32A32Sfloat;

    case VertexAttributeType::Byte4:
        return isNormalized ? vk::Format::eR8G8B8A8Unorm : vk::Format::eR8G8B8A8Uint;

    case VertexAttributeType::Int10Pack:
        return vk::Format::eA2B10G10R10UnormPack32;
    default:
        std::unreachable();
    }
}

uint32_t getAttributeTypeSize(ly::renderer::VertexAttributeType type)
{
    using ly::renderer::VertexAttributeType;
    switch (type)
    {
    case VertexAttributeType::Float:
        return 4;
    case VertexAttributeType::Float2:
        return 8;
    case VertexAttributeType::Float3:
        return 12;
    case VertexAttributeType::Float4:
        return 16;

    // 4 bytes: RGBA8 (1 byte per channel)
    case VertexAttributeType::Byte4:
    case VertexAttributeType::Byte4N:
        return 4;

    // 4 bytes: 2 Shorts (2 bytes per channel)
    case VertexAttributeType::Short2:
    case VertexAttributeType::Short2N:
        return 4;

    // 4 bytes: Packed 10-10-10-2 bit format
    case VertexAttributeType::Int10Pack:
        return 4;

    default:
        std::unreachable();
    }
}
} // namespace

namespace ly::renderer
{
BufferElement::BufferElement(VertexAttributeSlot slot, VertexAttributeType type, bool isNormalized) :
    slot(slot),
    type(type),
    isNormalized(isNormalized) {}

BufferLayout::BufferLayout(std::vector<BufferElement> elements) :
    m_elements(std::move(elements))
{
    calculateOffsetsAndStride();
}

/**
 * Can be optimized to make sure the stride compute doesn't start from scratch.
 */
void BufferLayout::addElement(BufferElement element)
{
    m_elements.push_back(std::move(element));
    updateStride();
}

std::vector<vk::VertexInputAttributeDescription>
    BufferLayout::getVertexAttributeDescriptions(uint32_t binding) const
{
    std::vector<vk::VertexInputAttributeDescription> result;
    result.reserve(m_elements.size());

    for (auto& element : m_elements)
    {
        result.push_back(
            vk::VertexInputAttributeDescription{
                .location = static_cast<uint32_t>(element.slot),
                .binding  = binding,
                .format   = getVulkanFormat(element.type, element.isNormalized),
                .offset   = element.offset
            });
    }

    return result;
}

void BufferLayout::calculateOffsetsAndStride()
{
    uint32_t offset = 0;
    for (auto& element : m_elements)
    {
        element.offset = offset;
        offset += getAttributeTypeSize(element.type);
    }
    m_stride = offset;
}

void BufferLayout::updateStride()
{
    m_elements.back().offset = m_stride;
    m_stride += getAttributeTypeSize(m_elements.back().type);
}

VertexBuffer::VertexBuffer(LogicalDevice const& device, BufferLayout const& layout, std::span<std::byte const> data) :
    m_layout(layout)
{
    vk::BufferCreateInfo bufferCreateInfo{
        .size        = data.size(),
        .usage       = vk::BufferUsageFlagBits::eVertexBuffer,
        .sharingMode = vk::SharingMode::eExclusive
    };
    auto expect = device.getHandle().createBuffer(bufferCreateInfo);
    assert(expect.has_value() && "Buffer creation failed");
    m_bufferHandle = std::move(expect.value());

    vk::MemoryRequirements memoryRequirements = m_bufferHandle.getMemoryRequirements();

    vk::MemoryAllocateInfo memoryAllocateInfo{
        .allocationSize  = memoryRequirements.size,
        .memoryTypeIndex = device.findMemoryType(
            memoryRequirements.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
    };

    auto expectMemory = device.getHandle().allocateMemory(memoryAllocateInfo);
    assert(expectMemory.has_value() && "Memory allocation failed");
    m_bufferHandle.bindMemory(*expectMemory, 0);

    void* vkData = expectMemory->mapMemory(0, data.size());
    memcpy(vkData, data.data(), data.size());
    expectMemory->unmapMemory();
}

vk::VertexInputBindingDescription VertexBuffer::getBindingDescription(uint32_t binding) const
{
    return {
        .binding   = binding,
        .stride    = m_layout.getStride(),
        .inputRate = vk::VertexInputRate::eVertex
    };
}
} // namespace ly::renderer