#include "Lightyear/Common/Math.h"
#include "Lightyear/Serialization/Text/TextSerialization.h"

namespace ly {

// -------- vec2 --------
template <>
struct SerializableAdapter<glm::vec2> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::vec2& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::vec2& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        deserializer.EndArray();
    }
};

// -------- vec3 --------
template <>
struct SerializableAdapter<glm::vec3, void> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::vec3& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.Write("", v.z);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::vec3& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.z);
        }
        deserializer.EndArray();
    }
};

// -------- vec4 --------
template <>
struct SerializableAdapter<glm::vec4> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::vec4& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.Write("", v.z);
        serializer.Write("", v.w);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::vec4& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.z);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.w);
        }
        deserializer.EndArray();
    }
};

// -------- uvec2 --------
template <>
struct SerializableAdapter<glm::uvec2> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::uvec2& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::uvec2& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        deserializer.EndArray();
    }
};

// -------- uvec3 --------
template <>
struct SerializableAdapter<glm::uvec3> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::uvec3& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.Write("", v.z);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::uvec3& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.z);
        }
        deserializer.EndArray();
    }
};

// -------- uvec4 --------
template <>
struct SerializableAdapter<glm::uvec4> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::uvec4& v) {
        serializer.BeginArray(key);
        serializer.Write("", v.x);
        serializer.Write("", v.y);
        serializer.Write("", v.z);
        serializer.Write("", v.w);
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::uvec4& v) {
        deserializer.BeginArray(key);
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.x);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.y);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.z);
        }
        if (deserializer.HasNextArrayElement()) {
            deserializer.Read("", v.w);
        }
        deserializer.EndArray();
    }
};

// -------- mat4 --------
// Represent as 4 vec4 columns
template <>
struct SerializableAdapter<glm::mat4> {
    static constexpr bool value = true;

    static void Serialize(TextSerializer& serializer, const std::string& key, const glm::mat4& m) {
        serializer.BeginArray(key);
        for (int i = 0; i < 4; ++i) {
            /// column based serialization
            SerializableAdapter<glm::vec4>::Serialize(serializer, std::format("col{}", i), m[i]);
        }
        serializer.EndArray();
    }

    static void Deserialize(TextDeserializer& deserializer, const std::string& key, glm::mat4& m) {
        deserializer.BeginArray(key);
        for (int i = 0; i < 4; ++i) {
            glm::vec4 col{};
            SerializableAdapter<glm::vec4>::Deserialize(deserializer, std::format("col{}", i), col);
            m[i] = col;
        }
        deserializer.EndArray();
    }
};

}  // namespace ly
