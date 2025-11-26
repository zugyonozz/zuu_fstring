#pragma once

/**
 * @file fstring_utils.hpp
 * @author zugyonozz (rafizuhayr001@gmail.com)
 * @brief Utility functions for fstring operations
 * @version 2.0.0
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 */

#include "fstring_traits.hpp"

namespace zuu {
namespace detail {

    /// Constexpr minimum function
    struct min {
        template <typename T, typename U>
        requires std::totally_ordered_with<T, U>
        [[nodiscard]] constexpr auto operator()(T a, U b) const noexcept {
            return (a < b) ? a : b;
        }
    };

    /// Constexpr maximum function
    struct max {
        template <typename T, typename U>
        requires std::totally_ordered_with<T, U>
        [[nodiscard]] constexpr auto operator()(T a, U b) const noexcept {
            return (a > b) ? a : b;
        }
    };

    /// Memory copy operation
    struct copy {
        template <typename T>
        constexpr void operator()(T* dst, const T* src, std::size_t count) const noexcept {
            if (dst == nullptr || src == nullptr || count == 0) return;
            for (std::size_t i = 0; i < count; ++i) {
                dst[i] = src[i];
            }
        }
    };

    /// Memory move operation (handles overlapping ranges)
    struct move {
        template <typename T>
        constexpr void operator()(T* dst, const T* src, std::size_t count) const noexcept {
            if (dst == nullptr || src == nullptr || count == 0) return;
            
            if (dst < src) {
                for (std::size_t i = 0; i < count; ++i) {
                    dst[i] = src[i];
                }
            } else if (dst > src) {
                for (std::size_t i = count; i > 0; --i) {
                    dst[i - 1] = src[i - 1];
                }
            }
        }
    };

    /// Fill memory with value
    struct fill {
        template <typename T>
        constexpr void operator()(T* dst, T value, std::size_t count) const noexcept {
            if (dst == nullptr) return;
            for (std::size_t i = 0; i < count; ++i) {
                dst[i] = value;
            }
        }
    };

    /// Compare two memory regions
    struct compare {
        template <typename T>
        [[nodiscard]] constexpr int operator()(const T* a, const T* b, std::size_t count) const noexcept {
            if (a == b || count == 0) return 0;
            if (a == nullptr) return -1;
            if (b == nullptr) return 1;
            
            for (std::size_t i = 0; i < count; ++i) {
                if (a[i] < b[i]) return -1;
                if (a[i] > b[i]) return 1;
            }
            return 0;
        }
    };

    /// Calculate string length
    template <char_t CharT>
    [[nodiscard]] constexpr std::size_t length(const CharT* str) noexcept {
        if (str == nullptr) return 0;
        std::size_t len = 0;
        while (str[len] != CharT{}) {
            ++len;
        }
        return len;
    }

    /// Find character in string
    template <char_t CharT>
    [[nodiscard]] constexpr const CharT* find(const CharT* str, std::size_t len, CharT ch) noexcept {
        if (str == nullptr) return nullptr;
        for (std::size_t i = 0; i < len; ++i) {
            if (str[i] == ch) return str + i;
        }
        return nullptr;
    }

    /// Find substring
    template <char_t CharT>
    [[nodiscard]] constexpr const CharT* find_str(const CharT* haystack, std::size_t hay_len, 
                                                   const CharT* needle, std::size_t needle_len) noexcept {
        if (haystack == nullptr || needle == nullptr) return nullptr;
        if (needle_len == 0) return haystack;
        if (needle_len > hay_len) return nullptr;
        
        for (std::size_t i = 0; i <= hay_len - needle_len; ++i) {
            bool match = true;
            for (std::size_t j = 0; j < needle_len; ++j) {
                if (haystack[i + j] != needle[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return haystack + i;
        }
        return nullptr;
    }

    /// Reverse find character
    template <char_t CharT>
    [[nodiscard]] constexpr const CharT* rfind(const CharT* str, std::size_t len, CharT ch) noexcept {
        if (str == nullptr || len == 0) return nullptr;
        for (std::size_t i = len; i > 0; --i) {
            if (str[i - 1] == ch) return str + i - 1;
        }
        return nullptr;
    }

    /// Check if character is whitespace
    template <char_t CharT>
    [[nodiscard]] constexpr bool is_space(CharT ch) noexcept {
        return ch == CharT(' ') || ch == CharT('\t') || 
               ch == CharT('\n') || ch == CharT('\r') ||
               ch == CharT('\f') || ch == CharT('\v');
    }

    /// Check if character is digit
    template <char_t CharT>
    [[nodiscard]] constexpr bool is_digit(CharT ch) noexcept {
        return ch >= CharT('0') && ch <= CharT('9');
    }

    /// Check if character is alphabetic
    template <char_t CharT>
    [[nodiscard]] constexpr bool is_alpha(CharT ch) noexcept {
        return (ch >= CharT('a') && ch <= CharT('z')) ||
               (ch >= CharT('A') && ch <= CharT('Z'));
    }

    /// Convert character to lowercase
    template <char_t CharT>
    [[nodiscard]] constexpr CharT to_lower(CharT ch) noexcept {
        if (ch >= CharT('A') && ch <= CharT('Z')) {
            return ch + (CharT('a') - CharT('A'));
        }
        return ch;
    }

    /// Convert character to uppercase
    template <char_t CharT>
    [[nodiscard]] constexpr CharT to_upper(CharT ch) noexcept {
        if (ch >= CharT('a') && ch <= CharT('z')) {
            return ch - (CharT('a') - CharT('A'));
        }
        return ch;
    }

    /// Count digits needed for integer
    template <std::integral IntT>
    [[nodiscard]] constexpr std::size_t count_digits(IntT value) noexcept {
        if (value == 0) return 1;
        
        std::size_t count = 0;
        if constexpr (std::is_signed_v<IntT>) {
            if (value < 0) {
                count = 1; // for minus sign
                value = -value;
            }
        }
        
        while (value > 0) {
            value /= 10;
            ++count;
        }
        return count;
    }

    /// Convert integer to string
    template <std::integral IntT, char_t CharT>
    constexpr void int_to_string(CharT* buffer, std::size_t size, IntT value) noexcept {
        if (buffer == nullptr || size == 0) return;
        
        std::size_t pos = 0;
        bool negative = false;
        
        if constexpr (std::is_signed_v<IntT>) {
            if (value < 0) {
                negative = true;
                value = -value;
            }
        }
        
        if (value == 0) {
            buffer[pos++] = CharT('0');
        } else {
            CharT temp[64];
            std::size_t temp_pos = 0;
            
            while (value > 0 && temp_pos < 64) {
                temp[temp_pos++] = CharT('0') + (value % 10);
                value /= 10;
            }
            
            if (negative && pos < size) {
                buffer[pos++] = CharT('-');
            }
            
            while (temp_pos > 0 && pos < size) {
                buffer[pos++] = temp[--temp_pos];
            }
        }
        
        if (pos < size) {
            buffer[pos] = CharT{};
        }
    }

    /// Convert floating point to string (simplified)
    template <std::floating_point FloatT, char_t CharT>
    constexpr void float_to_string(CharT* buffer, std::size_t size, FloatT value, int precision = 6) noexcept {
        if (buffer == nullptr || size == 0) return;
        
        std::size_t pos = 0;
        
        // Handle special cases
        if (value != value) { // NaN
            if (size >= 4) {
                buffer[0] = CharT('N');
                buffer[1] = CharT('a');
                buffer[2] = CharT('N');
                buffer[3] = CharT{};
            }
            return;
        }
        
        // Handle negative
        if (value < 0) {
            if (pos < size) buffer[pos++] = CharT('-');
            value = -value;
        }
        
        // Integer part
        auto int_part = static_cast<long long>(value);
        int_to_string(buffer + pos, size - pos, int_part);
        pos = length(buffer);
        
        // Decimal point
        if (pos < size && precision > 0) {
            buffer[pos++] = CharT('.');
            
            // Fractional part
            FloatT frac = value - static_cast<FloatT>(int_part);
            for (int i = 0; i < precision && pos < size - 1; ++i) {
                frac *= 10;
                int digit = static_cast<int>(frac);
                buffer[pos++] = CharT('0') + digit;
                frac -= digit;
            }
        }
        
        if (pos < size) {
            buffer[pos] = CharT{};
        }
    }

} // namespace detail
} // namespace zuu
