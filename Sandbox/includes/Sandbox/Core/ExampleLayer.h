#pragma once

#include <iostream>
#include "Lightyear.h"

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {}

    void OnEvent(ly::Event& event) override {}
};
