/**
 * @file constant_string_fwd.hpp
 * @brief Forward declarations untuk compile-time string library
 * @version 1.0
 */

#pragma once

#include <cstddef>
#include <type_traits>

namespace zuu {

// Forward declarations
template <typename CharT, std::size_t N>
class string;

template <typename CharT, bool IsConst>
class string_iterator;

namespace traits {
    template <typename T>
    struct is_char;
    
    template <typename T>
    inline constexpr bool is_char_v = is_char<std::remove_cv_t<T>>::value;
}

namespace detail {
    // Utility functions
    template <traits::character CharT>
    [[nodiscard]] constexpr std::size_t strlen(const CharT* str) noexcept;
    
    template <traits::character CharT>
    [[nodiscard]] constexpr int strcmp(const CharT* a, const CharT* b, 
                                        std::size_t len) noexcept;
}

// Type aliases
template <std::size_t N> using cstring    = string<char, N>;
template <std::size_t N> using cwstring   = string<wchar_t, N>;
template <std::size_t N> using cu8string  = string<char8_t, N>;
template <std::size_t N> using cu16string = string<char16_t, N>;
template <std::size_t N> using cu32string = string<char32_t, N>;

} // namespace zuu
