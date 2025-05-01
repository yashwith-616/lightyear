#pragma once

#include "Lightyear.h"

class Geometry {
public:
    ly::Ref<ly::renderer::VertexArray> m_CubeVertexArray;
    ly::Ref<ly::renderer::VertexArray> m_SphereVertexArray;
    ly::Ref<ly::renderer::VertexArray> m_CylinderVertexArray;
    ly::Ref<ly::renderer::VertexArray> m_TeapotVertexArray;
    ly::Ref<ly::renderer::VertexArray> m_PlaneVertexArray;

    Geometry();

    static void DrawPlane(ly::renderer::RendererAPI* renderAPI);

    static void DrawCube(ly::renderer::RendererAPI* renderAPI);

    static void DrawSphere(ly::renderer::RendererAPI* renderAPI);

    static void DrawCylinder(ly::renderer::RendererAPI* renderAPI);

    /**
     * @brief This is an OpenGL only function
     */
    static void DrawTeapot(ly::renderer::RendererAPI* renderAPI);

    static Geometry* s_Geometry;
};