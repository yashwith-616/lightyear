#include "Lightyear/Editor/ImGuiLayer.h"

namespace ly {

ImGuiLayer::~ImGuiLayer() {
    // Destroy ImGUI
}

void ImGuiLayer::OnAttach() {
    // Initialize ImGUI
}

void ImGuiLayer::OnDetach() {
    // Close Imgui
}

void ImGuiLayer::OnUpdate() {
    // Run the update function
}

void ImGuiLayer::OnEvent(Event& event) {}

}  // namespace ly
