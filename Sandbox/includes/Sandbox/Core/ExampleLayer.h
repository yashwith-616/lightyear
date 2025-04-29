#pragma once

#include <iostream>
#include "Lightyear.h"

namespace renderer = ly::renderer;

static std::array<std::array<float, 3>, 4> planeVertices{
    { { -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { -0.5f, 0.5f, 0.0f } }
};

static std::array<uint32_t, 6> planeIndices{ 0, 1, 2, 2, 3, 0 };

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {
        // Required Data
        float* vBufferData   = planeVertices.data()->data();
        uint32_t vBufferSize = static_cast<uint32_t>(planeVertices.size() * sizeof(planeVertices[0]));
        uint32_t iBufferSize = static_cast<uint32_t>(planeIndices.size() * sizeof(uint32_t));

        // Initiate Buffer Layout
        auto planeBufferLayout = { renderer::BufferElement(renderer::ShaderDataType::Float3, "Position", true) };

        // Initiate plane vertex buffer
        auto* planeVertexBuffer = renderer::VertexBuffer::Create(vBufferData, vBufferSize);
        planeVertexBuffer->SetLayout(planeBufferLayout);

        // Initiate Index Buffer
        auto* planeIndexBuffer = renderer::IndexBuffer::Create(planeIndices.data(), iBufferSize);

        // Create Vertex Array
        auto* planeVertexArray = renderer::VertexArray::Create();
        planeVertexArray->AddVertexBuffer(std::shared_ptr<renderer::VertexBuffer>(planeVertexBuffer));
        planeVertexArray->SetIndexBuffer(planeIndexBuffer);

        // Render
        auto* renderAPI = renderer::RendererAPI::Create();
        renderAPI->Init();
        renderAPI->Clear();
        renderAPI->DrawIndexed(std::shared_ptr<renderer::VertexArray>(planeVertexArray));
    }

    void OnEvent(ly::Event& event) override {}
};
