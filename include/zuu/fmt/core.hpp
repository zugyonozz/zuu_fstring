#pragma once

/**
 * @file zuu/fmt/core.hpp
 * @brief Modern formatting system with proxy objects
 * @version 3.0.0
 * 
 * Usage:
 *   auto s = to_fstring(42);              // "42"
 *   auto s = to_fstring(hex(255));         // "0xff"
 *   auto s = to_fstring(bin(5));           // "0b101"
 *   auto s = to_fstring(pad_left(42, 5));  // "00042"
 */

#include "../core/core.hpp"
#include "../meta/concepts.hpp"
#include <cmath>
#include <concepts>
#include <limits>

namespace zuu::fmt {

// ==================== Core Formatter (Extension Point) ====================

template <typename T>
struct formatter;

// Default formatter for integrals
template <std::integral T>
struct formatter<T> {
    template <meta::character CharT = char>
    static constexpr auto format(T value) noexcept {
        constexpr std::size_t max_digits = std::numeric_limits<T>::digits10 + 3;
        basic_fstring<CharT, max_digits> result;
        
        if (value == 0) {
            result.push_back(CharT('0'));
            return result;
        }
        
        bool negative = false;
        using UIntT = std::make_unsigned_t<T>;
        UIntT uvalue;
        
        if constexpr (std::is_signed_v<T>) {
            if (value < 0) {
                negative = true;
                uvalue = static_cast<UIntT>(-(value + 1)) + 1;
            } else {
                uvalue = static_cast<UIntT>(value);
            }
        } else {
            uvalue = value;
        }
        
        CharT buffer[max_digits];
        std::size_t pos = 0;
        
        while (uvalue > 0) {
            buffer[pos++] = CharT('0') + (uvalue % 10);
            uvalue /= 10;
        }
        
        if (negative) result.push_back(CharT('-'));
        
        while (pos > 0) {
            result.push_back(buffer[--pos]);
        }
        
        return result;
    }
};

// Default formatter for floating point
template <std::floating_point T>
struct formatter<T> {
    template <meta::character CharT = char>
    static constexpr auto format(T value, int precision = 6) noexcept {
        constexpr std::size_t max_size = 64;
        basic_fstring<CharT, max_size> result;
        
        // Constexpr-friendly NaN/Inf check (avoid std::isnan in constexpr)
        if (!std::is_constant_evaluated()) {
            if (std::isnan(value)) {
                result.append("nan", 3);
                return result;
            }
            if (std::isinf(value)) {
                if (value < 0) result.push_back(CharT('-'));
                result.append("inf", 3);
                return result;
            }
        }
        
        if (value < 0) {
            result.push_back(CharT('-'));
            value = -value;
        }
        
        auto int_part = static_cast<long long>(value);
        auto int_str = formatter<long long>::format(int_part);
        result.append(int_str.data(), int_str.size());
        
        if (precision > 0) {
            result.push_back(CharT('.'));
            T frac = value - static_cast<T>(int_part);
            
            for (int i = 0; i < precision && !result.full(); ++i) {
                frac *= 10;
                int digit = static_cast<int>(frac);
                result.push_back(CharT('0') + digit);
                frac -= digit;
            }
        }
        
        return result;
    }
};

// ==================== Hex Proxy ====================

template <std::integral T>
struct hex_proxy {
    T value;
    bool uppercase;

    constexpr hex_proxy(T v, bool upper = false) 
        : value{v}, uppercase{upper} {}
};

template <std::integral T>
constexpr auto hex(T value, bool uppercase = false) {
    return hex_proxy{value, uppercase};
}

template <std::integral T>
struct formatter<hex_proxy<T>> {
    template <meta::character CharT = char>
    static constexpr auto format(const hex_proxy<T>& proxy) noexcept {
        constexpr std::size_t max_size = sizeof(T) * 2 + 3;
        basic_fstring<CharT, max_size> result;
        result.append("0x", 2);
        
        using UIntT = std::make_unsigned_t<T>;
        UIntT uvalue = static_cast<UIntT>(proxy.value);
        
        CharT buffer[max_size];
        std::size_t pos = 0;
        
        do {
            int digit = uvalue % 16;
            if (digit < 10) {
                buffer[pos++] = CharT('0') + digit;
            } else {
                CharT base = proxy.uppercase ? CharT('A') : CharT('a');
                buffer[pos++] = base + (digit - 10);
            }
            uvalue /= 16;
        } while (uvalue > 0);
        
        while (pos > 0) {
            result.push_back(buffer[--pos]);
        }
        
        return result;
    }
};

// ==================== Binary Proxy ====================

template <std::integral T>
struct bin_proxy {
    T value;
    
    constexpr bin_proxy(T v) : value{v} {}
};

template <std::integral T>
constexpr auto bin(T value) {
    return bin_proxy{value};
}

template <std::integral T>
struct formatter<bin_proxy<T>> {
    template <meta::character CharT = char>
    static constexpr auto format(const bin_proxy<T>& proxy) noexcept {
        constexpr std::size_t max_size = sizeof(T) * 8 + 3;
        basic_fstring<CharT, max_size> result;
        result.append("0b", 2);
        
        using UIntT = std::make_unsigned_t<T>;
        UIntT uvalue = static_cast<UIntT>(proxy.value);
        
        CharT buffer[max_size];
        std::size_t pos = 0;
        
        do {
            buffer[pos++] = CharT('0') + (uvalue % 2);
            uvalue /= 2;
        } while (uvalue > 0);
        
        while (pos > 0) {
            result.push_back(buffer[--pos]);
        }
        
        return result;
    }
};

// ==================== Padding Proxy ====================

template <std::integral T>
struct pad_left_proxy {
    T value;
    std::size_t width;
    char fill;
    
    constexpr pad_left_proxy(T v, std::size_t w, char f = '0') 
        : value{v}, width{w}, fill{f} {}
};

template <std::integral T>
constexpr auto pad_left(T value, std::size_t width, char fill = '0') {
    return pad_left_proxy{value, width, fill};
}

template <std::integral T>
struct formatter<pad_left_proxy<T>> {
    template <meta::character CharT = char>
    static constexpr auto format(const pad_left_proxy<T>& proxy) noexcept {
        auto base_str = formatter<T>::template format<CharT>(proxy.value);
        
        constexpr std::size_t max_size = 64;
        basic_fstring<CharT, max_size> result;
        
        if (base_str.size() < proxy.width) {
            result.append(proxy.width - base_str.size(), CharT(proxy.fill));
        }
        result.append(base_str.data(), base_str.size());
        
        return result;
    }
};

// ==================== Boolean Formatter ====================

template <>
struct formatter<bool> {
    template <meta::character CharT = char>
    static constexpr auto format(bool value) noexcept {
        basic_fstring<CharT, 5> result;
        if (value) {
            result.append("true", 4);
        } else {
            result.append("false", 5);
        }
        return result;
    }
};

// ==================== Main Conversion Function ====================

template <typename T>
constexpr auto to_fstring(const T& value) noexcept {
    return formatter<std::decay_t<T>>::format(value);
}

// Overload for floating point with precision
template <std::floating_point T>
constexpr auto to_fstring(T value, int precision) noexcept {
    return formatter<T>::format(value, precision);
}

// ==================== Parsing ====================

template <std::integral IntT, meta::character CharT, std::size_t Cap>
constexpr IntT parse_int(const basic_fstring<CharT, Cap>& str, int base = 10) noexcept {
    if (str.empty()) return IntT{};
    
    IntT result = 0;
    std::size_t i = 0;
    bool negative = false;
    
    if constexpr (std::is_signed_v<IntT>) {
        if (str[0] == CharT('-')) {
            negative = true;
            ++i;
        } else if (str[0] == CharT('+')) {
            ++i;
        }
    }
    
    for (; i < str.size(); ++i) {
        CharT ch = str[i];
        int digit = -1;
        
        if (ch >= CharT('0') && ch <= CharT('9')) {
            digit = ch - CharT('0');
        } else if (ch >= CharT('a') && ch <= CharT('z')) {
            digit = 10 + (ch - CharT('a'));
        } else if (ch >= CharT('A') && ch <= CharT('Z')) {
            digit = 10 + (ch - CharT('A'));
        }
        
        if (digit < 0 || digit >= base) break;
        result = result * base + digit;
    }
    
    return negative ? -result : result;
}

template <std::floating_point FloatT, meta::character CharT, std::size_t Cap>
constexpr FloatT parse_float(const basic_fstring<CharT, Cap>& str) noexcept {
    if (str.empty()) return FloatT{};
    
    FloatT result = 0;
    std::size_t i = 0;
    bool negative = false;
    
    if (str[0] == CharT('-')) {
        negative = true;
        ++i;
    } else if (str[0] == CharT('+')) {
        ++i;
    }
    
    while (i < str.size() && str[i] >= CharT('0') && str[i] <= CharT('9')) {
        result = result * 10 + (str[i] - CharT('0'));
        ++i;
    }
    
    if (i < str.size() && str[i] == CharT('.')) {
        ++i;
        FloatT fraction = FloatT(0.1);
        while (i < str.size() && str[i] >= CharT('0') && str[i] <= CharT('9')) {
            result += (str[i] - CharT('0')) * fraction;
            fraction *= FloatT(0.1);
            ++i;
        }
    }
    
    return negative ? -result : result;
}

} // namespace zuu::fmt