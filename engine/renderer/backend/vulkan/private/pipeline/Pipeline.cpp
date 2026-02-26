#include <pipeline/Pipeline.h>

/// TODO:
///     1. Pipeline fixed state configuration must be different from the actual one
///     2. Shader stages must be configured outside the ecosystem of the pipelines

namespace ly::renderer
{

Pipeline::Pipeline(LogicalDevice const& device) : m_device(device) {}

void Pipeline::createPipeline(Swapchain const& swapchain, std::vector<vk::PipelineShaderStageCreateInfo> const& stages)
{
    auto inputState = configureVertexInputState();
    auto inputAssemblyState = configureInputAssemblyState();
    auto rasterizationState = configureRasterization();
    auto multisampleState = configureMultisample();
    auto colorBlendState = configureColorBlendState();

    auto dynamicStates = configureDynamicState();
    vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
        .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data(),
    };

    vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {
        .viewportCount = 1, .pViewports = {}, .scissorCount = 1, .pScissors = {}};

    vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .setLayoutCount = 0,
        .pushConstantRangeCount = 0,
    };

    auto expected = m_device.getHandle().createPipelineLayout(pipelineLayoutCreateInfo);
    assert(expected.has_value() && "Pipeline layout creation failed");
    vk::PipelineLayout pipelineLayout = expected.value();

    vk::GraphicsPipelineCreateInfo graphicPipelineCreateInfo = {
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

    auto viewportState = configureViewport(swapchain);
    vk::PipelineRenderingCreateInfo renderingCreateInfo = {
        .colorAttachmentCount = 1, .pColorAttachmentFormats = &swapchain.getSurfaceFormat().format};

    vk::StructureChain<vk::GraphicsPipelineCreateInfo, vk::PipelineRenderingCreateInfo> pipelineCreationInfo = {
        graphicPipelineCreateInfo, renderingCreateInfo};

    auto pipelineExpected = m_device.getHandle().createGraphicsPipeline(
        nullptr, pipelineCreationInfo.get<vk::GraphicsPipelineCreateInfo>());

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

vk::Viewport Pipeline::configureViewport(Swapchain const& swapchain)
{
    vk::Rect2D framebufferRectSize{.offset = vk::Offset2D(0, 0), .extent = swapchain.getExtent()};

    return vk::Viewport{
        .x = 0.f,
        .y = 0.f,
        .width = static_cast<float>(swapchain.getExtent().width),
        .height = static_cast<float>(swapchain.getExtent().height),
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

vk::PipelineColorBlendStateCreateInfo Pipeline::configureColorBlendState()
{
    auto colorBlendAttachmentState = configureColorBlendAttachment();

    return vk::PipelineColorBlendStateCreateInfo{
        .logicOpEnable = vk::False,
        .logicOp = vk::LogicOp::eCopy,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachmentState,
    };
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
