#pragma once

#include "Sandbox/Editor/Widgets/WidgetDrawerBase.h"
#include "Lightyear.h"

class SimpleWidgetDrawer : public WidgetDrawerBase<SimpleWidgetDrawer> {
public:
    SimpleWidgetDrawer() = default;
    ~SimpleWidgetDrawer() = default;

    template <typename T>
    void Draw(T& object) {
        if constexpr (refl::is_reflectable<T>()) {
            auto members = refl::reflect<T>().members;
            auto fields  = filter(members, [](auto member) { return is_field(member); });
            for_each(fields,
                     [&](auto member) { DrawWidget(member.name.c_str(), member(object)); });
        }
    }

private:
    template<typename T>
    void DrawWidget(std::string_view name, T& value) {
        LY_CORE_ASSERT(false, "Undefined behaviour for widget name {} of type {}", name, typeid(T).name());
    }
};

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, int& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value {}", name, value);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, float& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value {}", name, value);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, std::string& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value {}", name, value);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, ly::UUID& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value {}", name, value.Get());
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, glm::vec2& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value", name);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, glm::vec3& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value", name);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, glm::vec4& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value", name);
}

template<>
inline void SimpleWidgetDrawer::DrawWidget(std::string_view name, glm::mat4& value) {
    LY_LOG(ly::LogType::Info, "Draw widget with name {} and value", name);
}