#pragma once

/**
 * @file zuu/meta/traits.hpp
 * @brief Type traits for capacity and character detection
 * @version 3.0.0
 */

#include "concepts.hpp"
#include <span>

namespace zuu::meta {

// ==================== Capacity Detection ====================

// Primary template: dynamic string (no static capacity)
template <typename T>
struct capacity_of {
    static constexpr std::size_t value = std::dynamic_extent;
};

// Specialization for types with static capacity
template <has_static_capacity T>
struct capacity_of<T> {
    static constexpr std::size_t value = T::capacity;
};

template <typename T>
inline constexpr std::size_t capacity_of_v = capacity_of<T>::value;

// ==================== Character Type Detection ====================

// Primary template
template <typename T>
struct char_type_of {
    using type = void;
};

// Specialization for types with value_type
template <typename T>
requires requires { typename T::value_type; }
struct char_type_of<T> {
    using type = typename T::value_type;
};

// Specialization for raw pointers
template <character Char>
struct char_type_of<Char*> {
    using type = Char;
};

template <character Char>
struct char_type_of<const Char*> {
    using type = Char;
};

template <typename T>
using char_type_of_t = typename char_type_of<std::remove_cvref_t<T>>::type;

// ==================== Compatibility Checking ====================

template <typename T1, typename T2>
struct is_compatible_string : std::false_type {};

template <string_like T1, string_like T2>
requires std::same_as<char_type_of_t<T1>, char_type_of_t<T2>>
struct is_compatible_string<T1, T2> : std::true_type {};

template <typename T1, typename T2>
inline constexpr bool is_compatible_string_v = is_compatible_string<T1, T2>::value;

// ==================== Result Type Calculation ====================

// Calculate appropriate capacity for concatenation result
template <typename T1, typename T2>
struct concat_capacity {
    static constexpr std::size_t value = []() {
        constexpr auto c1 = capacity_of_v<T1>;
        constexpr auto c2 = capacity_of_v<T2>;
        
        if constexpr (c1 == std::dynamic_extent || c2 == std::dynamic_extent) {
            return std::dynamic_extent;
        } else {
            return c1 + c2;
        }
    }();
};

template <typename T1, typename T2>
inline constexpr std::size_t concat_capacity_v = concat_capacity<T1, T2>::value;

// ==================== Algorithm Result Type ====================

// For algorithms that preserve capacity
template <typename T>
struct preserve_capacity {
    static constexpr std::size_t value = capacity_of_v<T>;
};

// For algorithms that may expand (like split results)
template <typename T, std::size_t Multiplier = 2>
struct expand_capacity {
    static constexpr std::size_t value = []() {
        constexpr auto cap = capacity_of_v<T>;
        if constexpr (cap == std::dynamic_extent) {
            return std::dynamic_extent;
        } else {
            return cap * Multiplier;
        }
    }();
};

// ==================== SFINAE Helpers ====================

// Check if type is trivially copyable (optimization hint)
template <typename T>
inline constexpr bool is_trivial_string_v = 
    string_like<T> && 
    std::is_trivially_copyable_v<T>;

// Check if conversion is lossless
template <typename From, typename To>
inline constexpr bool is_lossless_conversion_v = 
    capacity_of_v<To> >= capacity_of_v<From>;

} // namespace zuu::meta