#pragma once

/**
 * @file fstring_literals.hpp
 * @author zugyonozz (rafizuhayr001@gmail.com)
 * @brief User-defined literals for fstring
 * @version 2.0.0
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 * Usage examples:
 *   auto str = "hello"_fs;           // fstring<5>
 *   auto wstr = L"hello"_fs;         // wfstring<5>
 *   auto u8str = u8"hello"_fs;       // u8fstring<5>
 *   auto u16str = u"hello"_fs;       // u16fstring<5>
 *   auto u32str = U"hello"_fs;       // u32fstring<5>
 */

#include "fstring_core.hpp"

namespace zuu {
inline namespace literals {
inline namespace fstring_literals {

/**
 * @brief User-defined literal for creating fstring from string literal
 * 
 * This uses a GCC/Clang extension that allows template parameters
 * to be characters from string literals.
 */
#if defined(__GNUC__) || defined(__clang__)

template <typename CharT, CharT... chars>
[[nodiscard]] constexpr auto operator""_fs() noexcept {
    constexpr CharT str[] = {chars..., CharT{}};
    constexpr std::size_t len = sizeof...(chars);
    
    if constexpr (std::is_same_v<CharT, char>) {
        return basic_fstring<char, len>(str, len);
    } else if constexpr (std::is_same_v<CharT, wchar_t>) {
        return basic_fstring<wchar_t, len>(str, len);
    } else if constexpr (std::is_same_v<CharT, char8_t>) {
        return basic_fstring<char8_t, len>(str, len);
    } else if constexpr (std::is_same_v<CharT, char16_t>) {
        return basic_fstring<char16_t, len>(str, len);
    } else if constexpr (std::is_same_v<CharT, char32_t>) {
        return basic_fstring<char32_t, len>(str, len);
    }
}

#endif

/**
 * @brief Standard user-defined literal (runtime size)
 * 
 * This version works with all compilers but uses a fixed maximum size.
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

/**
 * @brief Small string literal (optimized for short strings)
 */
[[nodiscard]] constexpr auto operator""_sfs(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 32>(str, len);
}

[[nodiscard]] constexpr auto operator""_sfs(const wchar_t* str, std::size_t len) noexcept {
    return basic_fstring<wchar_t, 32>(str, len);
}

/**
 * @brief Large string literal
 */
[[nodiscard]] constexpr auto operator""_lfs(const char* str, std::size_t len) noexcept {
    return basic_fstring<char, 1024>(str, len);
}

[[nodiscard]] constexpr auto operator""_lfs(const wchar_t* str, std::size_t len) noexcept {
    return basic_fstring<wchar_t, 1024>(str, len);
}

} // namespace fstring_literals
} // namespace literals
} // namespace zuu
