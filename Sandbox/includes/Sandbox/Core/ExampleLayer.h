#pragma once

#include <iostream>
#include "Lightyear.h"

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {
#pragma region Buffer Layout setup
        std::array<std::array<float, 3>, 4> planeVertices{ {
            { -0.5f, -0.5f, 0.0f },  // Bottom-left
            { 0.5f, -0.5f, 0.0f },   // Bottom-right
            { 0.5f, 0.5f, 0.0f },    // Top-right
            { -0.5f, 0.5f, 0.0f }    // Top-left
        } };

        std::array<uint32_t, 6> planeIndices{ 0, 1, 2, 2, 3, 0 };

        // Define the buffer element for vertex positions
        auto planeBufferElement =
            ly::renderer::BufferElement(ly::renderer::ShaderDataType::Float3, "Position", true);

        // Define buffer layout
        auto planeBufferLayout = { planeBufferElement };

        size_t val = static_cast<uint32_t>(planeVertices.size() * sizeof(planeVertices[0]));

        // Create vertex buffer and calculate the correct size in bytes
        ly::renderer::VertexBuffer* planeVertexBuffer = ly::renderer::VertexBuffer::Create(
            planeVertices.data()->data(),
            static_cast<uint32_t>(planeVertices.size() * sizeof(planeVertices[0])));
        planeVertexBuffer->SetLayout(planeBufferLayout);

        // Create index buffer
        ly::renderer::IndexBuffer* planeIndexBuffer = ly::renderer::IndexBuffer::Create(
            planeIndices.data(), static_cast<uint32_t>(planeIndices.size() * sizeof(uint32_t)));

        // Create vertex array and set buffers
        ly::renderer::VertexArray* planeVertexArray = ly::renderer::VertexArray::Create();
        planeVertexArray->AddVertexBuffer(
            std::shared_ptr<ly::renderer::VertexBuffer>(planeVertexBuffer));
        planeVertexArray->SetIndexBuffer(planeIndexBuffer);
#pragma endregion

        auto* renderAPI = ly::renderer::RendererAPI::Create();
        renderAPI->Init();
        renderAPI->Clear();
        renderAPI->DrawIndexed(*planeVertexArray);
    }

    void OnEvent(ly::Event& event) override {}
};
