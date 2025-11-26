#pragma once

/**
 * @file zuu/meta/concepts.hpp
 * @brief Core concepts for type constraints
 * @version 3.0.0
 */

#include <concepts>
#include <string_view>

namespace zuu::meta {

// ==================== Character Type Concepts ====================

template <typename T>
concept character = 
    std::same_as<T, char> ||
    std::same_as<T, wchar_t> ||
    std::same_as<T, char8_t> ||
    std::same_as<T, char16_t> ||
    std::same_as<T, char32_t>;

// ==================== String-Like Detection ====================

template <typename T>
concept has_data_and_size = requires(T t) {
    { t.data() } -> std::convertible_to<const typename T::value_type*>;
    { t.size() } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept has_c_str = requires(T t) {
    { t.c_str() } -> std::convertible_to<const typename T::value_type*>;
};

template <typename T>
concept convertible_to_string_view = requires(T t) {
    { std::basic_string_view{t} } -> std::same_as<std::basic_string_view<typename T::value_type>>;
};

// Main StringLike concept
template <typename T>
concept string_like = 
    has_data_and_size<T> || 
    convertible_to_string_view<T>;

// ==================== Fixed-Capacity String Detection ====================

template <typename T>
concept has_static_capacity = requires {
    { T::capacity } -> std::convertible_to<std::size_t>;
};

template <typename T>
concept fixed_string = 
    string_like<T> && 
    has_static_capacity<T>;

// ==================== Algorithm Composability ====================

// Detect if type supports piping (has operator|)
template <typename T, typename Fn>
concept pipeable_with = requires(T val, Fn fn) {
    { val | fn };
};

// Callable with string-like argument
template <typename F, typename Str>
concept string_transformer = 
    string_like<Str> &&
    std::invocable<F, Str>;

// ==================== Formatting Support ====================

template <typename T>
concept formattable = requires(T val) {
    // Will be specialized per type
    requires std::integral<T> || 
             std::floating_point<T> || 
             string_like<T>;
};

// ==================== Range Concepts ====================

template <typename R>
concept string_range = 
    std::ranges::range<R> &&
    string_like<std::ranges::range_value_t<R>>;

} // namespace zuu::meta