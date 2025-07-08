#include "Lightyear/Core/Layer.h"
#include "Lightyear/Events/Event.h"

namespace ly {

Layer::Layer(std::string debugName) : m_DebugName(std::move(debugName)) {}

void Layer::OnAttach() {}

void Layer::OnDetach() {}

void Layer::OnEvent(Event& event) {}

void Layer::OnUpdate(float deltaTime) {}

void Layer::OnEditorRender() {}

}  // namespace ly