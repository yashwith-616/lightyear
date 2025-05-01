#pragma once

#include <array>

// ----- Cube Vertex Position -----
constexpr float g_CubeVertices[] = { -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                                     -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f };

// ----- Cube Index Position -----
constexpr uint32_t g_CubeIndices[] = {
    0, 1, 2, 2, 3, 0,  // back face
    4, 5, 6, 6, 7, 4,  // front face
    4, 5, 1, 1, 0, 4,  // bottom face
    7, 6, 2, 2, 3, 7,  // top face
    4, 0, 3, 3, 7, 4,  // left face
    5, 1, 2, 2, 6, 5   // right face
};

static std::array<std::array<float, 3>, 4> g_PlaneVertices{
    { { -0.5f, -0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { -0.5f, 0.5f, 0.0f } }
};

static std::array<uint32_t, 6> g_PlaneIndices{ 0, 1, 2, 2, 3, 0 };

constexpr size_t g_CubeVertexCount = sizeof(g_CubeVertices);
constexpr size_t g_CubeIndexCount  = sizeof(g_CubeIndices);

constexpr size_t g_PlaneVertexCount = g_PlaneVertices.size() * sizeof(g_PlaneVertices[0]);
constexpr size_t g_PlaneIndexCount  = g_PlaneIndices.size() * sizeof(uint32_t);
