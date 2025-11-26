#pragma once

/**
 * @file zuu/core/literals.hpp
 * @brief User-defined literals for convenient fstring creation
 * @version 3.0.0
 * 
 * Usage:
 *   using namespace zuu::literals;
 *   auto s = "hello"_fs;      // fstring<256>
 *   auto s = "hi"_sfs;        // fstring<32> (small)
 *   auto s = "big"_lfs;       // fstring<1024> (large)
 */

#include "core.hpp"

namespace zuu::inline literals::inline fstring_literals {

// ==================== Standard Literals (runtime size) ====================

/**
 * @brief Standard fstring literal (default capacity: 256)
 */
[[nodiscard]] constexpr auto operator""_fs(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 256>(str, len);
}

[[nodiscard]] constexpr auto operator""_fs(const wchar_t* str, std::size_t len) noexcept {
    return basic_fstring<wchar_t, 256>(str, len);
}

[[nodiscard]] constexpr auto operator""_fs(const char8_t* str, std::size_t len) noexcept {
    return basic_fstring<char8_t, 256>(str, len);
}

[[nodiscard]] constexpr auto operator""_fs(const char16_t* str, std::size_t len) noexcept {
    return basic_fstring<char16_t, 256>(str, len);
}

[[nodiscard]] constexpr auto operator""_fs(const char32_t* str, std::size_t len) noexcept {
    return basic_fstring<char32_t, 256>(str, len);
}

// ==================== Small String Literals ====================

/**
 * @brief Small fstring literal (capacity: 32)
 * Optimized for short strings like names, identifiers
 */
[[nodiscard]] constexpr auto operator""_sfs(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 32>(str, len);
}

[[nodiscard]] constexpr auto operator""_sfs(const wchar_t* str, std::size_t len) noexcept {
    return basic_fstring<wchar_t, 32>(str, len);
}

// ==================== Large String Literals ====================

/**
 * @brief Large fstring literal (capacity: 1024)
 * For longer text content
 */
[[nodiscard]] constexpr auto operator""_lfs(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 1024>(str, len);
}

[[nodiscard]] constexpr auto operator""_lfs(const wchar_t* str, std::size_t len) noexcept {
    return basic_fstring<wchar_t, 1024>(str, len);
}

// ==================== Compile-Time Sized Literals (GCC/Clang Extension) ====================

#if defined(__GNUC__) || defined(__clang__)

/**
 * @brief Compile-time exact-sized literal (GCC/Clang only)
 * Automatically deduces capacity from literal length
 * 
 * Usage: auto s = "hello"_fs;  // fstring<5>
 */
template <typename CharT, CharT... chars>
[[nodiscard]] constexpr auto operator""_cfs() noexcept {
    constexpr CharT str[] = {chars..., CharT{}};
    constexpr std::size_t len = sizeof...(chars);
    return basic_fstring<CharT, len>(str, len);
}

#endif // GCC/Clang

// ==================== Specialized Purpose Literals ====================

/**
 * @brief Path string literal (capacity: 260 for MAX_PATH)
 */
[[nodiscard]] constexpr auto operator""_path(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 260>(str, len);
}

/**
 * @brief UUID string literal (capacity: 36)
 */
[[nodiscard]] constexpr auto operator""_uuid(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 36>(str, len);
}

/**
 * @brief IP address literal (capacity: 45 for IPv6)
 */
[[nodiscard]] constexpr auto operator""_ip(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 45>(str, len);
}

/**
 * @brief URL string literal (capacity: 2048)
 */
[[nodiscard]] constexpr auto operator""_url(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 2048>(str, len);
}

} // namespace zuu::inline literals::inline fstring_literals