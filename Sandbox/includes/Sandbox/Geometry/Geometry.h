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

    static ly::Ref<ly::renderer::VertexArray> GetPlane();

    static ly::Ref<ly::renderer::VertexArray> GetCube();

    static ly::Ref<ly::renderer::VertexArray> GetSphere();

    static ly::Ref<ly::renderer::VertexArray> GetCylinder();

    /**
     * @brief This is an OpenGL only function
     */
    static ly::Ref<ly::renderer::VertexArray> GetTeapot();

    static Geometry* s_Geometry;
};