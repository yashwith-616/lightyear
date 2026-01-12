#pragma once

#include "Lightyear.h"

class Geometry {
public:
    ly::Ref<ly::renderer::VertexArray> CubeVertexArray;
    ly::Ref<ly::renderer::VertexArray> SphereVertexArray;
    ly::Ref<ly::renderer::VertexArray> CylinderVertexArray;
    ly::Ref<ly::renderer::VertexArray> TeapotVertexArray;
    ly::Ref<ly::renderer::VertexArray> PlaneVertexArray;

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
