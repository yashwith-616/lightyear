#pragma once

#include "Lightyear.h"
#include "Sandbox/Editor/Widgets/WidgetDrawerBase.h"

class WidgetDrawer : public WidgetDrawerBase<WidgetDrawer>
{
public:
    WidgetDrawer() = default;
    ~WidgetDrawer() = default;

    template <typename T>
    bool Draw(T& object)
    {
        bool changed = false;

        if constexpr (refl::is_reflectable<T>())
        {
            auto members = refl::reflect<T>().members;
            auto fields = filter(members, [](auto member) { return is_field(member); });

            for_each(fields, [&](auto member) { changed |= DrawWidget(member.name.c_str(), member(object)); });
        }

        return changed;
    }

private:
    template <typename T>
    bool DrawWidget(std::string_view name, T& value)
    {
        LY_CORE_ASSERT(false, "Undefined behaviour for widget name {} of type {}", name, typeid(T).name());
        return false;
    }

    template <typename T>
        requires std::is_enum_v<T>
    bool DrawWidget(std::string_view name, T& value)
    {
        using Underlying = std::underlying_type_t<T>;
        ImGui::Text("%s: %llu", name.data(), static_cast<unsigned long long>(static_cast<Underlying>(value)));
        return false;
    }
};
template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, int& value)
{
    return ImGui::DragInt(name.data(), &value, 1);
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, float& value)
{
    return ImGui::DragFloat(name.data(), &value, 0.1f);
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, std::string& value)
{
    char buffer[256];
    std::strncpy(buffer, value.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    if (ImGui::InputText(name.data(), buffer, sizeof(buffer)))
    {
        value = buffer;
        return true;
    }
    return false;
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, ly::Uuid& value)
{
    ImGui::Text("%s: %llu", name.data(), static_cast<unsigned long long>(value.get()));
    return false;
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, glm::vec2& value)
{
    return ImGui::DragFloat2(name.data(), glm::value_ptr(value), 0.1f);
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, glm::vec3& value)
{
    return ImGui::DragFloat3(name.data(), glm::value_ptr(value), 0.1f);
}

template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, glm::vec4& value)
{
    return ImGui::DragFloat4(name.data(), glm::value_ptr(value), 0.1f);
}
template <>
inline bool WidgetDrawer::DrawWidget(std::string_view name, glm::mat4& value)
{
    bool changed = false;

    ImGui::Text("%s:", name.data());
    ImGui::Indent();
    for (int i = 0; i < 4; ++i)
    {
        changed |=
            ImGui::DragFloat4(("##" + std::string(name) + std::to_string(i)).c_str(), glm::value_ptr(value[i]), 0.1f);
    }
    ImGui::Unindent();

    return changed;
}
