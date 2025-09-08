#pragma once

#include "TextSerialization.h"
LY_DISABLE_WARNINGS_PUSH
#include "yaml-cpp/yaml.h"
LY_DISABLE_WARNINGS_POP

namespace ly {

class LIGHTYEAR_API YamlTextSerializer final : TextSerializer {
public:
    YamlTextSerializer(std::filesystem::path filepath);

protected:
    void WriteImpl(const std::string& key, int64_t value) override;
    void WriteImpl(const std::string& key, uint64_t value) override;
    void WriteImpl(const std::string& key, double value) override;
    void WriteImpl(const std::string& key, bool value) override;
    void WriteImpl(const std::string& key, const std::string& value) override;

    void BeginObject(const std::string& key) override;
    void EndObject() override;

    void BeginArray(const std::string& key) override;
    void EndArray() override;

    virtual void SaveToFile();

private:
    YAML::Node m_Root;
    std::stack<std::reference_wrapper<YAML::Node>> m_NodeStack;
    std::filesystem::path m_FilePath;
};

class LIGHTYEAR_API YamlTextDeserializer final : TextDeserializer {
public:
    YamlTextDeserializer(std::filesystem::path filepath);

protected:
    void ReadImpl(const std::string& key, uint64_t& value) override;
    void ReadImpl(const std::string& key, int64_t& value) override;
    void ReadImpl(const std::string& key, double& value) override;
    void ReadImpl(const std::string& key, bool& value) override;
    void ReadImpl(const std::string& key, std::string& value) override;

    void BeginObject(const std::string& key) override;
    void EndObject() override;

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
