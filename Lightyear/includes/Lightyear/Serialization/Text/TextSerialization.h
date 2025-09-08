#pragma once

#include "Lightyear/Common/Assertions.h"
#include "Lightyear/Serialization/Concepts.h"
#include "Lightyear/pch/lypch.h"

namespace ly {

/// \brief Base class for text based serialization
///
/// TextSerializer provides an interface to write objects to a text based format. It is designed to be extended for
/// specific output targets such as files and file formats such as json, yaml
class LIGHTYEAR_API TextSerializer {
public:
    virtual ~TextSerializer() = default;

    /// \brief Starts writing an array under the given key
    virtual void BeginArray(const std::string& key) = 0;

    /// \brief Ends writing to an array
    virtual void EndArray() = 0;

    /// \brief Write to a key-value pair to the text-based serialization output.
    /// \param key the identifier for the value being written
    /// \param value The string value to serialize.
    void Write(const std::string& key, const std::string& value) { WriteImpl(key, value); }

    /// \brief Write arithmetic values to the text-based serialization output. The method casts the values to higher
    /// precision values to not lose any data on save
    ///
    /// \tparam T the type is required to be arithmetic
    /// \param key the identifier for the value being written
    /// \param value the arithmetic value to be written
    template <typename T>
        requires(std::is_arithmetic_v<T> || std::is_same_v<T, bool>)
    void Write(const std::string& key, const T& value) {
        if constexpr (std::is_signed_v<T> && std::is_integral_v<T>) {
            WriteImpl(key, static_cast<int64_t>(value));
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            WriteImpl(key, static_cast<uint64_t>(value));
        } else if constexpr (std::is_floating_point_v<T>) {
            WriteImpl(key, static_cast<double>(value));
        } else if constexpr (std::is_same_v<T, bool>) {
            WriteImpl(key, value);
        } else {
            static_assert(false, "Not implemented yet!");
        }
    }

    /// \brief Writes a primitive type to the text-based serialization output.
    ///
    /// This method handles primitive types and third-party objects that are adapted via `SerializableAdapter`. Types
    /// that extend `SerializableAdapter`  and satisfy the `is_primitive_serializable` concept can be serialized using
    /// this method.
    ///
    /// \tparam T The type of the value being serialized. Must satisfy
    ///           `internal::is_primitive_serializable<T>`.
    /// \param key The identifier for the value in the serialized output.
    /// \param value The value to serialize. Types adapted via `SerializableAdapter`
    ///              are treated as primitive for serialization purposes.
    template <typename T>
        requires internal::is_primitive_serializable<T>::value
    void Write(const std::string& key, const T& value) {
        SerializableAdapter<T>::Serialize(*this, key, value);
    }

    /// \brief Writes a container type to the text-based serialization output.
    ///
    /// \tparam T The type of the container. Must satisfy `internal::is_container_type_v<T>`.
    /// \param key The identifier for the container in the serialized output.
    /// \param value The container instance whose elements are to be serialized.
    template <typename T>
        requires internal::is_container_type_v<T>
    void Write(const std::string& key, const T& value) {
        BeginArray(key);
        for (const auto& element : value) {
            Write("", element);
        }
        EndArray();
    }

    /// \brief Writes a user-defined type that implements the serialization contract.
    ///
    /// This method serializes types that satisfy the `has_serialize_contract_v` concept. It wraps the output in an
    /// object structure, allowing nested fields to be written via the class's own `Serialize` method.
    ///
    /// \tparam T The type of the object being serialized. Must satisfy  `internal::has_serialize_contract_v<T>`.
    /// \param key The identifier for the object in the serialized output.
    /// \param value The instance of the object to serialize.
    ///
    /// \note The method asserts at compile-time that the class has a `version` field  and a static
    /// `Serialize(ITextWriter&, const T&)` function.
    template <typename T>
        requires internal::has_serialize_contract_v<T>
    void Write(const std::string& key, const T& value) {
        static_assert(internal::has_version_v<T>, "Serializable class missing 'version'");
        static_assert(internal::has_text_serialize_v<T>,
                      "Serializable class missing static 'Serialize(TextSerialize&, const T&)'");
        BeginObject(key);
        T::Serialize(*this, value);
        EndObject();
    }

    /// \brief Ensures a type that defines serialization members inherits SerializableContract.
    ///
    /// This method is a compile-time guard to prevent accidental misuse. It checks that a class defining serialization
    /// or deserialization members also inherits `SerializableContract`. If the check fails, a runtime assertion is
    /// triggered.
    ///
    /// \tparam T The type being checked for proper serialization contract adherence.
    /// \param key Unused parameter, kept for consistency with other `Write` overloads.
    /// \param value Unused parameter, kept for consistency with other `Write` overloads.
    template <typename T>
    void Write(const std::string& key, const T& value) {
        constexpr bool hasAnySerialization = internal::has_serialize_v<T> || internal::has_deserialize_v<T>;
        constexpr bool isContract          = internal::has_serialize_contract_v<T>;
        static_assert(!hasAnySerialization || isContract,
                      "Class defines serialization members but does not inherit SerializableContract");
    }

protected:
    virtual void WriteImpl(const std::string& key, int64_t value)            = 0;
    virtual void WriteImpl(const std::string& key, uint64_t value)           = 0;
    virtual void WriteImpl(const std::string& key, double value)             = 0;
    virtual void WriteImpl(const std::string& key, bool value)               = 0;
    virtual void WriteImpl(const std::string& key, const std::string& value) = 0;

    virtual void BeginObject(const std::string& key) = 0;
    virtual void EndObject()                         = 0;
};

/// \brief Base class for text based deserialization
///
/// TextDeserializer provides an interface to read objects from a text based format. It is designed to be extended for
/// specific targets such as files.
class LIGHTYEAR_API TextDeserializer {
public:
    virtual ~TextDeserializer() = default;

    /// \brief Starts reading an array under the given key.
    ///
    /// \param key The name of the array field to begin.
    virtual void BeginArray(const std::string& key) = 0;

    /// \brief Ends reading an array field to begin
    virtual void EndArray() = 0;

    /// \brief Check if the array that is currently read has next element
    virtual bool HasNextArrayElement() = 0;

    /// \brief Reads a string value from the text-based deserialization input.
    ///
    /// \param key The identifier for the value being read.
    /// \param value Output parameter where the deserialized string will be stored.
    virtual void Read(const std::string& key, std::string& value) { ReadImpl(key, value); }

    /// \brief Reads arithmetic values from the text-based deserialization input.
    ///
    /// \tparam T The type to read. Must be arithmetic.
    /// \param key The identifier for the value being read.
    /// \param value Output parameter where the deserialized value will be stored.
    template <typename T>
        requires std::is_arithmetic_v<T>
    void Read(const std::string& key, T& value) {
        if constexpr (std::is_signed_v<T> && std::is_integral_v<T>) {
            int64_t temp{};
            ReadImpl(key, temp);
            value = static_cast<T>(temp);
        } else if constexpr (std::is_unsigned_v<T> && std::is_integral_v<T>) {
            uint64_t temp{};
            ReadImpl(key, temp);
            value = static_cast<T>(temp);
        } else if constexpr (std::is_floating_point_v<T>) {
            double temp{};
            ReadImpl(key, temp);
            value = static_cast<T>(temp);
        } else if constexpr (std::is_same_v<T, bool>) {
            bool temp{};
            ReadImpl(key, temp);
            value = temp;
        } else {
            static_assert(false, "Not implemented yet!");
        }
    }

    /// \brief Reads a primitive type adapted via `SerializableAdapter`.
    ///
    /// \tparam T The type being deserialized. Must satisfy `internal::is_primitive_serializable<T>`.
    /// \param key The identifier for the value.
    /// \param value Output parameter where the deserialized value will be stored.
    template <typename T>
        requires internal::is_primitive_serializable<T>::value
    void Read(const std::string& key, T& value) {
        SerializableAdapter<T>::Deserialize(*this, key, value);
    }

    /// \brief Reads a container type from the text-based input.
    ///
    /// \tparam T The container type. Must satisfy `internal::is_container_type_v<T>`.
    /// \param key The identifier for the container.
    /// \param value Output parameter where the container will be filled with deserialized elements.
    template <typename T>
        requires internal::is_container_type_v<T>
    void Read(const std::string& key, T& value) {
        BeginArray(key);
        value.clear();
        while (HasNextArrayElement()) {
            typename T::value_type element{};
            Read("", element);
            value.push_back(element);
        }
        EndArray();
    }

    /// \brief Reads a user-defined type that implements the serialization contract.
    ///
    /// \tparam T The type of the object. Must satisfy `internal::has_serialize_contract_v<T>`.
    /// \param key The identifier for the object.
    /// \param value Output parameter where the deserialized object will be stored.
    template <typename T>
        requires internal::has_serialize_contract_v<T>
    void Read(const std::string& key, T& value) {
        static_assert(internal::has_version_v<T>, "Deserializable class missing 'version'");
        static_assert(internal::has_text_deserialize_v<T>,
                      "Serializable class missing static 'Deserialize(TextDeserializer&, T&)'");

        BeginObject(key);
        T::Deserialize(*this, value);
        EndObject();
    }

    /// \brief Ensures a type defining serialization members inherits SerializableContract.
    ///
    /// \tparam T The type being checked.
    /// \param key Unused, kept for consistency with other `Read` overloads.
    /// \param value Unused, kept for consistency.
    template <typename T>
    void Read(const std::string& /*key*/, T& /*value*/) {
        constexpr bool hasAnySerialization = internal::has_serialize_v<T> || internal::has_deserialize_v<T>;
        constexpr bool isContract          = internal::has_serialize_contract_v<T>;
        static_assert(!hasAnySerialization || isContract,
                      "Class defines serialization members but does not inherit SerializableContract");
    }

protected:
    virtual void ReadImpl(const std::string& key, int64_t& value)     = 0;
    virtual void ReadImpl(const std::string& key, uint64_t& value)    = 0;
    virtual void ReadImpl(const std::string& key, double& value)      = 0;
    virtual void ReadImpl(const std::string& key, bool& value)        = 0;
    virtual void ReadImpl(const std::string& key, std::string& value) = 0;

    virtual void BeginObject(const std::string& key) = 0;
    virtual void EndObject()                         = 0;
};

}  // namespace ly
