#include "pipeline/Pipeline.h"

/// TODO:
///     1. Pipeline fixed state configuration must be different from the actual one
///     2. Shader stages must be configured outside the ecosystem of the pipelines

namespace ly::renderer
{

Pipeline::Pipeline(LogicalDevice const& device) : m_device(device) {}

void Pipeline::createPipeline(
    std::vector<vk::PipelineShaderStageCreateInfo> const& stages,
    vk::Extent2D extent,
    vk::SurfaceFormatKHR const& surface)
{
    auto inputState = configureVertexInputState();
    auto inputAssemblyState = configureInputAssemblyState();
    auto rasterizationState = configureRasterization();
    auto multisampleState = configureMultisample();
    auto colorBlendAttachmentState = configureColorBlendAttachment();

    auto colorBlendState = vk::PipelineColorBlendStateCreateInfo{
        .logicOpEnable = vk::False,
        .logicOp = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachmentState,
    };

    auto dynamicStates = configureDynamicState();
    auto dynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data(),
    };

    auto viewportStateCreateInfo =
        vk::PipelineViewportStateCreateInfo{.viewportCount = 1, .pViewports = {}, .scissorCount = 1, .pScissors = {}};

    auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0,
    };

    auto expected = m_device.getHandle().createPipelineLayout(pipelineLayoutCreateInfo);
    assert(expected.has_value() && "Pipeline layout creation failed");
    vk::PipelineLayout pipelineLayout = std::move(expected.value());

    auto graphicPipelineCreateInfo = vk::GraphicsPipelineCreateInfo{
        .stageCount = static_cast<uint32_t>(stages.size()),
        .pStages = stages.data(),
        .pVertexInputState = &inputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = pipelineLayout,
        .renderPass = nullptr};

    auto viewportState = configureViewport(extent);
    vk::PipelineRenderingCreateInfo renderingCreateInfo = {
        .colorAttachmentCount = 1, .pColorAttachmentFormats = &surface.format};
    graphicPipelineCreateInfo.pNext = &renderingCreateInfo;

    auto pipelineExpected = m_device.getHandle().createGraphicsPipeline(nullptr, graphicPipelineCreateInfo);

    assert(pipelineExpected.has_value() && "Pipeline creation failed");
    m_pipeline = std::move(pipelineExpected.value());
}

std::vector<vk::DynamicState> Pipeline::configureDynamicState()
{
    return {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };
}

vk::Viewport Pipeline::configureViewport(vk::Extent2D extent)
{
    vk::Rect2D framebufferRectSize{.offset = vk::Offset2D(0, 0), .extent = extent};

    return vk::Viewport{
        .x = 0.f,
        .y = 0.f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.f,
        .maxDepth = 1.f};
}

vk::PipelineRasterizationStateCreateInfo Pipeline::configureRasterization()
{
    return vk::PipelineRasterizationStateCreateInfo{
        .depthClampEnable = vk::False,
        .rasterizerDiscardEnable = vk::False,
        .polygonMode = vk::PolygonMode::eFill,
        .cullMode = vk::CullModeFlagBits::eBack,
        .frontFace = vk::FrontFace::eClockwise,
        .depthBiasEnable = vk::False,
        .depthBiasSlopeFactor = 1.0,
        .lineWidth = 1.f};
}

vk::PipelineMultisampleStateCreateInfo Pipeline::configureMultisample()
{
    return vk::PipelineMultisampleStateCreateInfo{
        .rasterizationSamples = vk::SampleCountFlagBits::e1, .sampleShadingEnable = vk::False};
}

vk::PipelineColorBlendAttachmentState Pipeline::configureColorBlendAttachment()
{
    return vk::PipelineColorBlendAttachmentState{
        .blendEnable = vk::False,
        .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
}

vk::PipelineInputAssemblyStateCreateInfo Pipeline::configureInputAssemblyState()
{
    return vk::PipelineInputAssemblyStateCreateInfo{
        .topology = vk::PrimitiveTopology::eTriangleList,
    };
}

vk::PipelineVertexInputStateCreateInfo Pipeline::configureVertexInputState()
{
    return vk::PipelineVertexInputStateCreateInfo{};
}


} // namespace ly::renderer
