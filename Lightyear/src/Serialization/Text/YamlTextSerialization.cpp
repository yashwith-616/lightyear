#include "Lightyear/Serialization/Text/YamlTextSerialization.h"

namespace ly {

/// ---------------------------------------------
/// Yaml Text Serializer
/// ---------------------------------------------
YamlTextSerializer::YamlTextSerializer(std::filesystem::path filepath)
    : m_Root(YAML::NodeType::Map), m_FilePath(std::move(filepath)) {
    m_NodeStack.push(std::ref(m_Root));
}

void YamlTextSerializer::WriteImpl(const std::string& key, int64_t value) {
    m_NodeStack.top()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, uint64_t value) {
    m_NodeStack.top()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, double value) {
    m_NodeStack.top()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, bool value) {
    m_NodeStack.top()[key] = value;
}

void YamlTextSerializer::WriteImpl(const std::string& key, const std::string& value) {
    m_NodeStack.top()[key] = value;
}

void YamlTextSerializer::BeginObject(const std::string& key) {
    YAML::Node newNode(YAML::NodeType::Map);
    m_NodeStack.top()[key] = newNode;
    m_NodeStack.push(std::ref(newNode));
}

void YamlTextSerializer::EndObject() {
    LY_CORE_ASSERT(!m_NodeStack.empty(), "Mismatched EndObject call");
    m_NodeStack.pop();
}

void YamlTextSerializer::BeginArray(const std::string& key) {
    YAML::Node newNode(YAML::NodeType::Sequence);
    m_NodeStack.top()[key] = newNode;
    m_NodeStack.push(std::ref(newNode));
}

void YamlTextSerializer::EndArray() {
    LY_CORE_ASSERT(!m_NodeStack.empty(), "Mismatched EndArray call");
}

void YamlTextSerializer::SaveToFile() {
    LY_CORE_LOG(LogType::INFO, "Saving yaml data to file {}", m_FilePath.string());

    std::ofstream file;
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    file.open(m_FilePath, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        LY_CORE_LOG(LogType::Error, "Failed to open YAML file for writing: {}", m_FilePath.string());
        return;
    }

    file << m_Root;
}

/// ---------------------------------------------
/// Yaml Text Deserializer
/// ---------------------------------------------
YamlTextDeserializer::YamlTextDeserializer(std::filesystem::path filepath) : m_FilePath(std::move(filepath)) {
    try {
        m_Root = YAML::LoadFile(m_FilePath.string());
        m_NodeStack.push(m_Root);
        m_ArrayIndexStack.push(0);
    } catch (const YAML::BadFile& e) {
        LY_CORE_ASSERT(false, "YAML bad file error '{}': {}", m_FilePath.string(), e.what());
    } catch (const YAML::ParserException& e) {
        LY_CORE_ASSERT(false, "YAML parsing error in '{}': {}", m_FilePath.string(), e.what());
    }
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
    LY_CORE_ASSERT(!m_NodeStack.empty(), "Mismatched EndObject call");
    LY_CORE_ASSERT(!m_ArrayIndexStack.empty(), "Mismatched EndObject array index");
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
