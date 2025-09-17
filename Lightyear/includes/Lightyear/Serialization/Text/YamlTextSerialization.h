#pragma once

#include "TextSerialization.h"
LY_DISABLE_WARNINGS_PUSH
#include "yaml-cpp/yaml.h"
LY_DISABLE_WARNINGS_POP

namespace ly {

class LIGHTYEAR_API YamlTextSerializer : public TextSerializer {
public:
    YamlTextSerializer(std::filesystem::path filepath);

    void SaveToFile() const;

    void BeginObject(const std::string& key) override;
    void EndObject() override;

protected:
    void WriteImpl(const std::string& key, int64_t value) override;
    void WriteImpl(const std::string& key, uint64_t value) override;
    void WriteImpl(const std::string& key, double value) override;
    void WriteImpl(const std::string& key, bool value) override;
    void WriteImpl(const std::string& key, const std::string& value) override;

    void BeginArray(const std::string& key) override;
    void EndArray() override;

private:
    YAML::Node m_Root;
    std::stack<YAML::Node> m_NodeStack;
    std::filesystem::path m_FilePath;
};

class LIGHTYEAR_API YamlTextDeserializer : public TextDeserializer {
public:
    YamlTextDeserializer(std::filesystem::path filepath);

    void BeginObject(const std::string& key) override;
    void EndObject() override;

protected:
    void ReadImpl(const std::string& key, uint64_t& value) override;
    void ReadImpl(const std::string& key, int64_t& value) override;
    void ReadImpl(const std::string& key, double& value) override;
    void ReadImpl(const std::string& key, bool& value) override;
    void ReadImpl(const std::string& key, std::string& value) override;

    void BeginArray(const std::string& key) override;
    void EndArray() override;
    bool HasNextArrayElement() override;

private:
    YAML::Node m_Root;
    std::stack<YAML::Node> m_NodeStack;
    std::stack<size_t> m_ArrayIndexStack;

    std::filesystem::path m_FilePath;
};

}  // namespace ly

LY_DISABLE_WARNINGS_PUSH
#include "entt/entt.hpp"
LY_DISABLE_WARNINGS_POP

inline bool RegisterSerializer() {
    static bool once = [] {
        using namespace entt::literals;
        entt::meta_factory<ly::YamlTextSerializer>()
            .type(entt::hashed_string{ "YamlSerializer" })
            .base<ly::TextSerializer>();
        return true;
    }();
    return once;
}

inline bool RegisterDeserializer() {
    static bool once = [] {
        using namespace entt::literals;
        entt::meta_factory<ly::YamlTextDeserializer>()
            .type(entt::hashed_string{ "YamlDeserializer" })
            .base<ly::TextDeserializer>();
        return true;
    }();
    return once;
}

inline bool transformSerializer   = RegisterSerializer();
inline bool transformDeserializer = RegisterDeserializer();