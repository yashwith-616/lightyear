#pragma once

#include <Lightyear.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <refl.hpp>

class EditWidgetDrawer {
public:
    bool field(const char* name, bool& value) { return ImGui::Checkbox(name, &value); }

    bool field(const char* name, int& value) { return ImGui::DragInt(name, &value); }

    bool field(const char* name, float& value) { return ImGui::DragFloat(name, &value); }

    bool field(const char* name, glm::vec3& value) { return ImGui::DragFloat3(name, &value.x); }

    bool field(const char* name, glm::quat& value) {
        // Display quaternion as euler angles.
        glm::vec3 angles = glm::degrees(glm::eulerAngles(value));
        if (ImGui::InputFloat3(name, &angles.x)) {
            value = glm::quat(radians(angles));
            return true;
        }
        return false;
    }

    bool field(const char* name, std::string_view value) {
        ImGui::Text("%s:", name);
        ImGui::SameLine();
        ImGui::TextWrapped("%s", value.data());
        return false;
    }

    bool field(const char* name, std::string& value) { return InputText(name, &value); }

    template <typename T>
    bool operator()(T& object) {
        bool changed = false;

        if constexpr (refl::is_reflectable<T>()) {
            auto members = refl::reflect<T>().members;
            auto fields  = filter(members, [](auto member) { return is_field(member); });
            for_each(fields,
                     [&](auto member) { changed |= field(member.name.c_str(), member(object)); });
        }

        return changed;
    }

private:
    // Fallback to silently accept all types that are not drawable.
    template <typename T>
    bool field(const char*, T&) {
        return false;
    }

private:
    bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0) {
        static char temp_buffer[1024];
        strncpy(temp_buffer, str->c_str(), sizeof(temp_buffer) - 1);
        temp_buffer[sizeof(temp_buffer) - 1] = '\0';

        bool changed = ImGui::InputText(label, temp_buffer, sizeof(temp_buffer), flags);
        if (changed && strlen(temp_buffer) > 0) {
            *str = temp_buffer;
        }
        return changed;
    }
};