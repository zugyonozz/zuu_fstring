#pragma once

/**
 * @file zuu/fstring.hpp
 * @brief Main entry point for fstring library v3.0
 * @author zugyonozz (rafizuhayr001@gmail.com)
 * @version 3.0.0
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 * Modern C++20 fixed-capacity string library with:
 * - Zero-cost abstractions
 * - Functional pipe composition
 * - Compile-time formatting
 * - std::ranges compatibility
 * 
 * Quick Start:
 * @code
 * #include <zuu/fstring.hpp>
 * using namespace zuu;
 * using namespace zuu::str;
 * using namespace zuu::fmt;
 * 
 * // Basic usage
 * fstring<32> name = "Alice";
 * 
 * // Functional piping
 * auto result = name 
 *     | trim 
 *     | to_upper 
 *     | [](auto s) { return s + "!!!"; };
 * 
 * // Modern formatting
 * auto hex_str = to_fstring(hex(255));        // "0xff"
 * auto bin_str = to_fstring(bin(42));         // "0b101010"
 * auto pad_str = to_fstring(pad_left(7, 4));  // "0007"
 * @endcode
 */

// ==================== Version Info ====================

namespace zuu {
    inline namespace version {
        constexpr int major = 3;
        constexpr int minor = 0;
        constexpr int patch = 0;
        constexpr const char* string = "3.0.0";
        
        constexpr bool is_at_least(int maj, int min = 0, int pat = 0) noexcept {
            if (major > maj) return true;
            if (major < maj) return false;
            if (minor > min) return true;
            if (minor < min) return false;
            return patch >= pat;
        }
    }
}

// ==================== Core Components ====================

// Meta-programming foundation
#include "meta/concepts.hpp"
#include "meta/traits.hpp"

// Core storage
#include "core/core.hpp"
#include "core/literals.hpp"

// String algorithms (pipeable)
#include "str/pipe.hpp"
#include "str/trim.hpp"
#include "str/case.hpp"
#include "str/split.hpp"
#include "str/find.hpp"

// Formatting system
#include "fmt/core.hpp"

// ==================== Convenience Namespace ====================

namespace zuu {

/**
 * @namespace zuu::types
 * @brief Common fixed-capacity string type aliases
 */
namespace types {
    // Standard sizes
    using str8 = fstring<8>;
    using str16 = fstring<16>;
    using str32 = fstring<32>;
    using str64 = fstring<64>;
    using str128 = fstring<128>;
    using str256 = fstring<256>;
    using str512 = fstring<512>;
    using str1k = fstring<1024>;
    
    // Specialized purposes
    using path_str = fstring<260>;      // MAX_PATH (Windows)
    using name_str = fstring<64>;       // Names, identifiers
    using msg_str = fstring<256>;       // Messages, descriptions
    using uuid_str = fstring<36>;       // UUID strings
    using ip_str = fstring<45>;         // IPv6 addresses
    using datetime_str = fstring<32>;   // ISO 8601 timestamps
    using url_str = fstring<2048>;      // URLs
    using email_str = fstring<254>;     // Email addresses (RFC 5321)
} // namespace types

} // namespace zuu

// ==================== User-Defined Literals ====================

// Literals are already in inline namespace from fstring/literals.hpp
// Just bring them into scope when users do: using namespace zuu::literals;

// ==================== Usage Documentation ====================

/**
 * @page usage Usage Guide
 * 
 * @section sec_basic Basic Operations
 * @code
 * zuu::fstring<32> str = "Hello";
 * str += " World";
 * std::cout << str << '\n';  // "Hello World"
 * @endcode
 * 
 * @section sec_pipe Functional Piping
 * @code
 * using namespace zuu::str;
 * 
 * auto result = "  HELLO WORLD  "
 *     | trim
 *     | to_lower
 *     | [](auto s) { 
 *         zuu::fstring<64> r = "Processed: ";
 *         r += s;
 *         return r;
 *       };
 * @endcode
 * 
 * @section sec_format Modern Formatting
 * @code
 * using namespace zuu::fmt;
 * 
 * auto s1 = to_fstring(255);           // "255"
 * auto s2 = to_fstring(hex(255));      // "0xff"
 * auto s3 = to_fstring(bin(5));        // "0b101"
 * auto s4 = to_fstring(pad_left(7, 3)); // "007"
 * @endcode
 * 
 * @section sec_constexpr Compile-Time Operations
 * @code
 * constexpr zuu::fstring<16> compile_time() {
 *     zuu::fstring<16> s = "test";
 *     return s | zuu::str::to_upper;
 * }
 * 
 * static_assert(compile_time() == "TEST");
 * @endcode
 */

/**
 * @page migration Migration from v2.0
 * 
 * @section sec_breaking Breaking Changes
 * 
 * 1. **Namespace restructuring**:
 *    - Old: `zuu::algorithms::to_upper(str)`
 *    - New: `zuu::str::to_upper(str)` or `str | zuu::str::to_upper`
 * 
 * 2. **Formatting changes**:
 *    - Old: `to_hex(255)`
 *    - New: `to_fstring(hex(255))`
 * 
 * 3. **Include paths**:
 *    - Old: `#include "fstring.hpp"`
 *    - New: `#include <zuu/fstring.hpp>`
 * 
 * @section sec_compat Compatibility Layer
 * Define `ZUU_FSTRING_COMPAT_V2` before including to enable v2 aliases.
 */

#ifdef ZUU_FSTRING_COMPAT_V2
namespace zuu {
    namespace algorithms = str;  // Alias for old code
}
#endif