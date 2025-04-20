#include "Lightyear/Core/Layer.h"
#include "Lightyear/Events/Event.h"

namespace ly {

Layer::Layer(const std::string& debugName) : m_DebugName(debugName) {}

Layer::~Layer() {}

void Layer::OnAttach() {}

void Layer::OnDetach() {}

void Layer::OnEvent(Event& event) {}

void Layer::OnUpdate() {}

}  // namespace ly