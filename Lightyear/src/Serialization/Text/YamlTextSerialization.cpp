#include "Lightyear/Serialization/Text/YamlTextSerialization.h"

namespace ly {

/// ---------------------------------------------
/// Yaml Text Serializer
/// ---------------------------------------------
YamlTextSerializer::YamlTextSerializer() : m_Root(YAML::NodeType::Map) {
    m_NodeStack.push(std::ref(m_Root));
}

void YamlTextSerializer::WriteImpl(const std::string& key, int64_t value) {
    m_NodeStack.top().get()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, uint64_t value) {
    m_NodeStack.top().get()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, double value) {
    m_NodeStack.top().get()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, bool value) {
    m_NodeStack.top().get()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, const std::string& value) {
    m_NodeStack.top().get()[key] = value;
}

void YamlTextSerializer::BeginObject(const std::string& key) {
    YAML::Node newNode(YAML::NodeType::Map);
    m_NodeStack.top().get()[key] = newNode;
    m_NodeStack.push(std::ref(newNode));
}

void YamlTextSerializer::EndObject() {
    LY_CORE_ASSERT(m_NodeStack.empty(), "Mismatched EndObject call");
    m_NodeStack.pop();
}

void YamlTextSerializer::BeginArray(const std::string& key) {
    YAML::Node newNode(YAML::NodeType::Sequence);
    m_NodeStack.top().get()[key] = newNode;
    m_NodeStack.push(std::ref(newNode));
}

void YamlTextSerializer::EndArray() {
    LY_CORE_ASSERT(m_NodeStack.empty(), "Mismatched EndArray call");
}

/// ---------------------------------------------
/// Yaml Text Deserializer
/// ---------------------------------------------
YamlTextDeserializer::YamlTextDeserializer() : m_Root(YAML::NodeType::Map) {
    m_NodeStack.push(std::ref(m_Root));
    m_ArrayIndexStack.push(0);
}

void YamlTextDeserializer::ReadImpl(const std::string& key, uint64_t& value) {
    value = m_NodeStack.top()[key].as<uint64_t>();
}

void YamlTextDeserializer::ReadImpl(const std::string& key, int64_t& value) {
    value = m_NodeStack.top()[key].as<int64_t>();
}

void YamlTextDeserializer::ReadImpl(const std::string& key, double& value) {
    value = m_NodeStack.top()[key].as<double>();
}

void YamlTextDeserializer::ReadImpl(const std::string& key, bool& value) {
    value = m_NodeStack.top()[key].as<bool>();
}

void YamlTextDeserializer::ReadImpl(const std::string& key, std::string& value) {
    value = m_NodeStack.top()[key].as<std::string>();
}

void YamlTextDeserializer::BeginObject(const std::string& key) {
    m_NodeStack.push(m_NodeStack.top()[key]);
    m_ArrayIndexStack.push(0);
}

void YamlTextDeserializer::EndObject() {
    LY_CORE_ASSERT(m_NodeStack.empty(), "Mismatched EndObject call");
    LY_CORE_ASSERT(m_ArrayIndexStack.empty(), "Mismatched EndObject array index");
    m_NodeStack.pop();
    m_ArrayIndexStack.pop();
}

void YamlTextDeserializer::BeginArray(const std::string& key) {
    m_NodeStack.push(m_NodeStack.top()[key]);
    m_ArrayIndexStack.push(0);
}

void YamlTextDeserializer::EndArray() {
    LY_CORE_ASSERT(m_NodeStack.empty(), "Mismatched EndArray call");
    LY_CORE_ASSERT(m_ArrayIndexStack.empty(), "Mismatched EndArray index");
    m_NodeStack.pop();
    m_ArrayIndexStack.pop();
}

bool YamlTextDeserializer::HasNextArrayElement() {
    LY_CORE_ASSERT(m_NodeStack.empty() && m_ArrayIndexStack.empty(), "Array context invalid in HasNextArrayElement");
    return m_ArrayIndexStack.top() < m_NodeStack.top().size();
}

}  // namespace ly
