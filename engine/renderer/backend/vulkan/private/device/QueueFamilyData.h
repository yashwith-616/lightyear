#pragma once
#include <bitset>
#include <cstdint>

namespace ly::renderer
{

enum class QueueSlot : uint8_t
{
    Graphic = 0,
    Compute = 1,
    Transfer = 2,
    Present = 3,
    Max = 4
};

enum class QueueTypeFlag : uint8_t
{
    None = 0,
    Graphic = 1 << 0,
    Compute = 1 << 1,
    Transfer = 1 << 2,
    Present = 1 << 3,
    All = Graphic | Compute | Transfer | Present
};

// Enable bitwise OR: Graphic | Compute
constexpr QueueTypeFlag operator|(QueueTypeFlag lhs, QueueTypeFlag rhs)
{
    return static_cast<QueueTypeFlag>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

// Enable bitwise AND: Flags & Graphic
constexpr QueueTypeFlag operator&(QueueTypeFlag lhs, QueueTypeFlag rhs)
{
    return static_cast<QueueTypeFlag>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

// Enable bitwise NOT: ~Graphic
constexpr QueueTypeFlag operator~(QueueTypeFlag rhs)
{
    return static_cast<QueueTypeFlag>(~static_cast<uint8_t>(rhs) & 0x0F);
}

// Compound assignment: flags |= Graphic
inline QueueTypeFlag& operator|=(QueueTypeFlag& lhs, QueueTypeFlag rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

/**
 * Checks if EVERY flag in 'required' is present in 'value'.
 * Useful for finding a "Universal" queue.
 */
constexpr bool hasAll(QueueTypeFlag value, QueueTypeFlag required) { return (value & required) == required; }

/**
 * Checks if AT LEAST ONE flag in 'subset' is present in 'value'.
 * Useful for checking if a queue can do "Any kind of work" (e.g., Graphic OR Compute).
 */
constexpr bool hasAny(QueueTypeFlag value, QueueTypeFlag subset) { return static_cast<uint8_t>(value & subset) != 0; }

/**
 * Checks if lhs and rhs are equal
 */
constexpr bool test(QueueTypeFlag lhs, QueueTypeFlag rhs)
{
    return (static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)) != 0;
}

/**
 * Returns a string representation (Great for logging/debugging).
 */
inline char const* toString(QueueTypeFlag type)
{
    switch (type)
    {
    case QueueTypeFlag::Graphic:
        return "Graphic";
    case QueueTypeFlag::Compute:
        return "Compute";
    case QueueTypeFlag::Transfer:
        return "Transfer";
    case QueueTypeFlag::Present:
        return "Present";
    case QueueTypeFlag::All:
        return "All (Universal)";
    default:
        return "Undefined";
    }
}

struct QueueFamilyData
{
    std::uint32_t familyIndex{};   // The "Family" index from the physical device
    std::uint32_t internalIndex{}; // The specific queue index within that family
    QueueTypeFlag flags{};
    float priority = 1.0f;

    bool hasSupport(QueueTypeFlag type) const
    {
        return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(type)) != 0;
    }

    void addSupport(QueueTypeFlag type) { flags |= type; }
};

} // namespace ly::renderer
