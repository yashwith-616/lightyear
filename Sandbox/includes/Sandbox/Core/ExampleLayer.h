#pragma once

#include "Lightyear.h"
#include <iostream>

class ExampleLayer : public ly::Layer {
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        std::cout << "Example update\n";
    }

    void OnEvent(ly::Event& event) override
    {
        std::cout << "On Event: " << event.ToString() << '\n';
    }
};
