/**
 * @file constant_string.hpp
 * @brief Compile-time fixed-size string library untuk C++20
 * @version 1.0
 * @author zuu
 * 
 * Library ini menyediakan implementasi string dengan ukuran tetap
 * yang dapat dievaluasi pada waktu kompilasi (constexpr).
 * 
 * Features:
 * - Full constexpr support (C++20)
 * - Zero-overhead abstractions
 * - Optimized algorithms
 * - STL-compatible interface
 * - Type-safe character handling
 * - Integer/float to_string conversions
 * - std::string interoperability
 * - Hash support
 * - Stream I/O
 * - User-defined literals
 * 
 * @example Basic Usage
 * ```cpp
 * #include "constant_string.hpp"
 * using namespace zuu::literals;
 * 
 * constexpr auto str = "Hello"_cs;
 * static_assert(str.length() == 5);
 * 
 * constexpr auto greeting = str + " World!"_cs;
 * static_assert(greeting == "Hello World!"_cs);
 * ```
 * 
 * @example Conversions
 * ```cpp
 * constexpr auto num = zuu::to_string(42);
 * static_assert(num == "42"_cs);
 * 
 * int value;
 * zuu::from_string(num, value);
 * assert(value == 42);
 * ```
 * 
 * @example std::string Interop
 * ```cpp
 * zuu::cstring<20> cs("Hello");
 * std::string std_str = zuu::to_std_string(cs);
 * auto back = zuu::from_std_string<20>(std_str);
 * ```
 */

#pragma once

// C++ Standard Library
#include <cstddef>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <iterator>
#include <compare>
#include <stdexcept>
#include <string>
#include <string_view>
#include <iostream>
#include <array>
#include <cmath>
#include <limits>

// Library Components (in dependency order)
#include "constant_string_fwd.hpp"
#include "constant_string_traits.hpp"
#include "constant_string_detail.hpp"
#include "constant_string_iterator.hpp"
#include "constant_string_class.hpp"
#include "constant_string_conversions.hpp"
#include "constant_string_literals.hpp"

/**
 * @namespace zuu
 * @brief Root namespace untuk constant string library
 */
namespace zuu {

/**
 * @brief Version information
 */
struct version {
    static constexpr int major = 1;
    static constexpr int minor = 0;
    static constexpr int patch = 0;
    
    [[nodiscard]] static constexpr const char* string() noexcept {
        return "1.0.0";
    }
    
    [[nodiscard]] static constexpr int number() noexcept {
        return major * 10000 + minor * 100 + patch;
    }
};

} // namespace zuu

/**
 * @namespace zuu::traits
 * @brief Type traits dan concepts
 */

/**
 * @namespace zuu::detail
 * @brief Internal implementation details
 */

/**
 * @namespace zuu::literals
 * @brief User-defined literals (_cs, _ws, etc.)
 */


// ============================================================================
// Documentation
// ============================================================================

/**
 * @mainpage Constant String Library
 * 
 * @section intro_sec Introduction
 * 
 * A modern C++20 compile-time string library with zero-overhead abstractions.
 * Provides a fixed-size string class that can be fully evaluated at compile-time
 * with optimized algorithms and full STL compatibility.
 * 
 * @section features_sec Key Features
 * 
 * - **Compile-time evaluation**: Full constexpr support
 * - **Type safety**: Strong typing with concepts
 * - **Zero overhead**: No dynamic allocation
 * - **STL compatible**: Works with algorithms and ranges
 * - **Rich API**: Comprehensive string operations
 * - **Conversions**: to_string, from_string, std::string interop
 * - **Modern C++**: C++20 features (concepts, spaceship operator)
 * 
 * @section usage_sec Usage Examples
 * 
 * @subsection basic_usage Basic String Operations
 * ```cpp
 * using namespace zuu::literals;
 * 
 * constexpr auto str = "Hello"_cs;
 * constexpr auto greeting = str + " World!"_cs;
 * static_assert(greeting.length() == 12);
 * static_assert(greeting.contains("World"));
 * ```
 * 
 * @subsection conversions Type Conversions
 * ```cpp
 * constexpr auto num = zuu::to_string(42);
 * constexpr auto pi = zuu::to_string(3.14159, 2);
 * constexpr auto hex = zuu::to_string(255, 16);
 * ```
 * 
 * @subsection algorithms String Algorithms
 * ```cpp
 * zuu::cstring<50> text("  Hello World  ");
 * text.trim().to_upper();
 * // text == "HELLO WORLD"
 * 
 * auto reversed = zuu::reversed(text);
 * // reversed == "DLROW OLLEH"
 * ```
 * 
 * @subsection interop STL Interoperability
 * ```cpp
 * zuu::cstring<100> cs("Hello");
 * std::string std_str = zuu::to_std_string(cs);
 * auto view = zuu::to_string_view(cs);
 * ```
 * 
 * @section performance_sec Performance
 * 
 * - **Compile-time**: Zero runtime cost for constexpr operations
 * - **Runtime**: Optimized with loop unrolling and efficient algorithms
 * - **Memory**: Stack-allocated, no heap fragmentation
 * - **Cache**: Contiguous memory layout for cache efficiency
 * 
 * @section requirements_sec Requirements
 * 
 * - C++20 compiler (GCC 10+, Clang 12+, MSVC 19.28+)
 * - Standard library with full C++20 support
 * 
 * @section license_sec License
 * 
 * MIT License or your preferred license here.
 */

#endif // CONSTANT_STRING_HPP
