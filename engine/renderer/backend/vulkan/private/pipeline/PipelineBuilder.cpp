/**
 * @file PipelineBuilder.cpp
 * @author Yashwith
 * @brief
 * @date 2026-03-05
 */
#include "PipelineBuilder.h"

#include <entt/meta/node.hpp>

#include "device/LogicalDevice.h"

namespace
{
struct BlendStrategy
{
    static vk::PipelineColorBlendAttachmentState opaque()
    {
        return {
            .blendEnable    = vk::False,
            .colorWriteMask = vk::ColorComponentFlags(0xb1111)
        };
    }

    static vk::PipelineColorBlendAttachmentState alpha()
    {
        return {
            .blendEnable         = vk::True,
            .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp        = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor = vk::BlendFactor::eZero,
            .alphaBlendOp        = vk::BlendOp::eAdd,
            .colorWriteMask      = vk::ColorComponentFlags(0xb1111)
        };
    }
};
}

namespace ly::renderer
{
struct PipelineConfig
{
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
    vk::PipelineRasterizationStateCreateInfo rasterization{};
    vk::PipelineMultisampleStateCreateInfo multisample{};

    // Must entirely be user controlled, if the user wants to be able to write custom shader impl
    vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
    vk::PipelineDepthStencilStateCreateInfo depthStencil{};
    std::vector<vk::DynamicState> dynamicStates;
    vk::raii::PipelineLayout layout{nullptr};

    vk::PipelineVertexInputStateCreateInfo vertexInputState{};
};

PipelineBuilder::PipelineBuilder(LogicalDevice const& device) :
    m_device(device),
    m_config(std::make_unique<PipelineConfig>())
{
    m_config->inputAssembly = vk::PipelineInputAssemblyStateCreateInfo{
        .topology               = vk::PrimitiveTopology::eTriangleList,
        .primitiveRestartEnable = vk::False
    };

    m_config->rasterization = vk::PipelineRasterizationStateCreateInfo{
        .depthClampEnable        = vk::False,
        .rasterizerDiscardEnable = vk::False,
        .polygonMode             = vk::PolygonMode::eFill,
        .cullMode                = vk::CullModeFlagBits::eBack,
        .frontFace               = vk::FrontFace::eClockwise,
        .depthBiasEnable         = vk::False,
        .lineWidth               = 1.f
    };

    m_config->colorBlendAttachment = vk::PipelineColorBlendAttachmentState{
        .blendEnable    = vk::False,
        .colorWriteMask = vk::ColorComponentFlags(0xb1111)
    };

    m_config->depthStencil = vk::PipelineDepthStencilStateCreateInfo{
        .depthTestEnable       = vk::True,
        .depthWriteEnable      = vk::True,
        .depthCompareOp        = vk::CompareOp::eLess,
        .depthBoundsTestEnable = vk::False,
        .stencilTestEnable     = vk::False,
        .front                 = {},
        .back                  = {},
        .minDepthBounds        = 0.0f,
        .maxDepthBounds        = 1.0f,
    };

    m_config->multisample = vk::PipelineMultisampleStateCreateInfo{
        .rasterizationSamples  = vk::SampleCountFlagBits::e1,
        .sampleShadingEnable   = vk::False,
        .minSampleShading      = 1.f,
        .alphaToCoverageEnable = vk::False,
        .alphaToOneEnable      = vk::False
    };

    m_config->dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
}

PipelineBuilder::~PipelineBuilder() = default;

PipelineBuilder& PipelineBuilder::withTopology(vk::PrimitiveTopology topology)
{
    m_config->inputAssembly.topology = topology;
    return *this;
}

PipelineBuilder& PipelineBuilder::withBlendMode(ColorBlendType type)
{
    switch (type)
    {
    case ColorBlendType::Opaque:
        m_config->colorBlendAttachment = BlendStrategy::opaque();
        break;
    case ColorBlendType::AlphaBlend:
        m_config->colorBlendAttachment = BlendStrategy::alpha();
        break;
    }

    return *this;
}

PipelineBuilder& PipelineBuilder::withCullMethod(vk::CullModeFlags mode)
{
    m_config->rasterization.cullMode = mode;
    return *this;
}

PipelineBuilder& PipelineBuilder::withPolygonMode(vk::PolygonMode mode)
{
    m_config->rasterization.polygonMode = mode;
    return *this;
}

PipelineBuilder& PipelineBuilder::withDepthTest(bool enable, std::optional<vk::CompareOp> op)
{
    m_config->depthStencil.depthTestEnable = enable ? vk::True : vk::False;
    m_config->depthStencil.depthWriteEnable = enable ? vk::True : vk::False;
    m_config->depthStencil.depthCompareOp = op.value_or(vk::CompareOp::eLess);
    return *this;
}

PipelineBuilder& PipelineBuilder::withRasterizationDepthBias(
    float depthBiasConstantFactor,
    float depthBiasClamp,
    float depthBiasSlopeFactor)
{
    m_config->rasterization.depthBiasEnable = vk::True;
    m_config->rasterization.depthBiasConstantFactor = depthBiasConstantFactor;
    m_config->rasterization.depthBiasClamp = depthBiasClamp;
    m_config->rasterization.depthBiasSlopeFactor = depthBiasSlopeFactor;

    return *this;
}


PipelineBuilder& PipelineBuilder::withMultisampleCount(vk::SampleCountFlagBits sampleCount)
{
    m_config->multisample.rasterizationSamples = sampleCount;
    return *this;
}

PipelineBuilder& PipelineBuilder::withLayout(vk::PipelineLayoutCreateInfo layoutInfo)
{
    auto layoutExpected = m_device.getHandle().createPipelineLayout(layoutInfo);
    assert(layoutExpected.has_value() && "Pipeline layout creation failed");
    m_config->layout = std::move(layoutExpected.value());
    return *this;
}

PipelineBuilder& PipelineBuilder::withAlphaToCoverageEnable(bool enable)
{
    m_config->multisample.alphaToCoverageEnable = enable;
    return *this;
}

vk::raii::Pipeline
    PipelineBuilder::build(
        std::vector<vk::PipelineShaderStageCreateInfo> const& stages,
        vk::PipelineVertexInputStateCreateInfo vertexInputState,
        vk::PipelineLayoutCreateInfo layoutInfo,
        vk::Format colorFormat)
{
    assert(m_config->layout != nullptr && "Layout has not been initialized for the pipeline");

    auto colorBlendState = vk::PipelineColorBlendStateCreateInfo{
        .logicOpEnable   = vk::False,
        .logicOp         = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments    = &m_config->colorBlendAttachment
    };

    auto dynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{
        .dynamicStateCount = static_cast<uint32_t>(m_config->dynamicStates.size()),
        .pDynamicStates    = m_config->dynamicStates.data(),
    };

    auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo{
        .viewportCount = 1,
        .pViewports    = nullptr,
        .scissorCount  = 1,
        .pScissors     = nullptr
    };

    auto renderingCreateInfo = vk::PipelineRenderingCreateInfo{
        .colorAttachmentCount    = 1,
        .pColorAttachmentFormats = &colorFormat,
        // TOD: Get the below formats from frame images
        .depthAttachmentFormat   = vk::Format::eUndefined,
        .stencilAttachmentFormat = vk::Format::eUndefined
    };

    auto graphicPipelineCreateInfo = vk::GraphicsPipelineCreateInfo{
        .pNext               = &renderingCreateInfo,
        .stageCount          = static_cast<uint32_t>(stages.size()),
        .pStages             = stages.data(),
        .pVertexInputState   = &vertexInputState,
        .pInputAssemblyState = &m_config->inputAssembly,
        .pTessellationState  = nullptr,
        .pViewportState      = &viewportStateCreateInfo,
        .pRasterizationState = &m_config->rasterization,
        .pMultisampleState   = &m_config->multisample,
        .pDepthStencilState  = &m_config->depthStencil,
        .pColorBlendState    = &colorBlendState,
        .pDynamicState       = &dynamicStateCreateInfo,
        .layout              = m_config->layout,
        .renderPass          = nullptr
    };

    auto expected = m_device.getHandle().createGraphicsPipeline(nullptr, graphicPipelineCreateInfo);
    assert(expected.has_value() && "Pipeline creation failed");
    return std::move(expected.value());
}
} // namespace ly::renderer