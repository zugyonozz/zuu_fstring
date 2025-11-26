#pragma once

/**
 * @file zuu/str/split.hpp
 * @brief Split and join operations with pipe support
 * @version 3.0.0
 * 
 * Usage:
 *   auto parts = split("a,b,c"_fs, ',');
 *   auto parts = "a,b,c"_fs | split(',');
 *   auto joined = join(parts, ", ");
 */

#include "../core/core.hpp"
#include "pipe.hpp"
#include <array>

namespace zuu::str {

// ==================== Split Result Container ====================

/**
 * @brief Container for split results
 * 
 * Provides array-like interface with actual count of parts
 */
template <meta::character CharT, std::size_t Cap, std::size_t MaxParts = 16>
struct split_result {
    basic_fstring<CharT, Cap> parts[MaxParts];
    std::size_t count = 0;
    
    // Container interface
    [[nodiscard]] constexpr auto begin() noexcept { return parts; }
    [[nodiscard]] constexpr auto end() noexcept { return parts + count; }
    [[nodiscard]] constexpr auto begin() const noexcept { return parts; }
    [[nodiscard]] constexpr auto end() const noexcept { return parts + count; }
    
    [[nodiscard]] constexpr std::size_t size() const noexcept { return count; }
    [[nodiscard]] constexpr bool empty() const noexcept { return count == 0; }
    
    [[nodiscard]] constexpr auto& operator[](std::size_t idx) noexcept {
        return parts[idx];
    }
    
    [[nodiscard]] constexpr const auto& operator[](std::size_t idx) const noexcept {
        return parts[idx];
    }
};

// ==================== Split by Character ====================

struct split_char_fn {
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts = 16>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap>& str, 
        CharT delimiter
    ) const noexcept {
        split_result<CharT, Cap, MaxParts> result;
        basic_fstring<CharT, Cap> current;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            if (str[i] == delimiter) {
                if (!current.empty() && result.count < MaxParts) {
                    result.parts[result.count++] = current;
                    current.clear();
                }
            } else {
                if (!current.full()) {
                    current.push_back(str[i]);
                }
            }
        }
        
        // Add last part if not empty
        if (!current.empty() && result.count < MaxParts) {
            result.parts[result.count++] = current;
        }
        
        return result;
    }
    
    // Factory for piping: str | split(',')
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT delimiter) const noexcept {
        return [delimiter, this](const auto& str) {
            return (*this)(str, delimiter);
        };
    }
};

inline constexpr split_char_fn split;

// ==================== Split by String ====================

struct split_str_fn {
    template <meta::character CharT, std::size_t Cap, std::size_t DelimCap, std::size_t MaxParts = 16>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap>& str,
        const basic_fstring<CharT, DelimCap>& delimiter
    ) const noexcept {
        split_result<CharT, Cap, MaxParts> result;
        
        if (delimiter.empty()) {
            if (result.count < MaxParts) {
                result.parts[result.count++] = str;
            }
            return result;
        }
        
        std::size_t pos = 0;
        
        while (pos < str.size() && result.count < MaxParts) {
            std::size_t found = str.find(delimiter.data(), pos);
            
            if (found == basic_fstring<CharT, Cap>::npos) {
                // No more delimiters, add remaining string
                basic_fstring<CharT, Cap> part;
                for (std::size_t i = pos; i < str.size() && !part.full(); ++i) {
                    part.push_back(str[i]);
                }
                if (!part.empty()) {
                    result.parts[result.count++] = part;
                }
                break;
            }
            
            // Add part before delimiter
            basic_fstring<CharT, Cap> part;
            for (std::size_t i = pos; i < found && !part.full(); ++i) {
                part.push_back(str[i]);
            }
            if (!part.empty()) {
                result.parts[result.count++] = part;
            }
            
            pos = found + delimiter.size();
        }
        
        return result;
    }
    
    // Factory for piping
    template <meta::character CharT, std::size_t DelimCap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, DelimCap>& delimiter) const noexcept {
        return [delimiter, this](const auto& str) {
            return (*this)(str, delimiter);
        };
    }
    
    // C-string overload
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* delimiter) const noexcept {
        return [delimiter, this](const auto& str) {
            basic_fstring<CharT, 64> delim(delimiter);
            return (*this)(str, delim);
        };
    }
};

inline constexpr split_str_fn split_by;

// ==================== Split Lines ====================

struct split_lines_fn : pipe_adaptor<split_lines_fn> {
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts = 16>
    [[nodiscard]] constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        split_result<CharT, Cap, MaxParts> result;
        basic_fstring<CharT, Cap> current;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            CharT ch = str[i];
            
            // Handle different line endings: \n, \r, \r\n
            if (ch == CharT('\n')) {
                if (!current.empty() && result.count < MaxParts) {
                    result.parts[result.count++] = current;
                    current.clear();
                }
            } else if (ch == CharT('\r')) {
                // Check for \r\n
                if (i + 1 < str.size() && str[i + 1] == CharT('\n')) {
                    ++i; // Skip the \n
                }
                if (!current.empty() && result.count < MaxParts) {
                    result.parts[result.count++] = current;
                    current.clear();
                }
            } else {
                if (!current.full()) {
                    current.push_back(ch);
                }
            }
        }
        
        // Add last line if not empty
        if (!current.empty() && result.count < MaxParts) {
            result.parts[result.count++] = current;
        }
        
        return result;
    }
};

inline constexpr split_lines_fn split_lines;

// ==================== Split Whitespace ====================

struct split_whitespace_fn : pipe_adaptor<split_whitespace_fn> {
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts = 16>
    [[nodiscard]] constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        split_result<CharT, Cap, MaxParts> result;
        basic_fstring<CharT, Cap> current;
        
        auto is_space = [](CharT ch) constexpr {
            return ch == CharT(' ') || ch == CharT('\t') || 
                   ch == CharT('\n') || ch == CharT('\r') ||
                   ch == CharT('\f') || ch == CharT('\v');
        };
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            if (is_space(str[i])) {
                if (!current.empty() && result.count < MaxParts) {
                    result.parts[result.count++] = current;
                    current.clear();
                }
            } else {
                if (!current.full()) {
                    current.push_back(str[i]);
                }
            }
        }
        
        // Add last part if not empty
        if (!current.empty() && result.count < MaxParts) {
            result.parts[result.count++] = current;
        }
        
        return result;
    }
};

inline constexpr split_whitespace_fn split_whitespace;

// ==================== Join Operations ====================

/**
 * @brief Join array of strings with delimiter
 */
struct join_fn {
    // Join with character delimiter
    template <meta::character CharT, std::size_t Cap, std::size_t N>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap> (&parts)[N],
        CharT delimiter
    ) const noexcept {
        constexpr std::size_t result_cap = Cap * N + N;
        basic_fstring<CharT, result_cap> result;
        
        for (std::size_t i = 0; i < N; ++i) {
            if (i > 0 && !result.full()) {
                result.push_back(delimiter);
            }
            if (!result.full()) {
                result.append(parts[i].data(), parts[i].size());
            }
        }
        
        return result;
    }
    
    // Join with string delimiter
    template <meta::character CharT, std::size_t Cap, std::size_t N, std::size_t DelimCap>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap> (&parts)[N],
        const basic_fstring<CharT, DelimCap>& delimiter
    ) const noexcept {
        constexpr std::size_t result_cap = Cap * N + DelimCap * N;
        basic_fstring<CharT, result_cap> result;
        
        for (std::size_t i = 0; i < N; ++i) {
            if (i > 0 && !result.full()) {
                result.append(delimiter.data(), delimiter.size());
            }
            if (!result.full()) {
                result.append(parts[i].data(), parts[i].size());
            }
        }
        
        return result;
    }
    
    // Join split_result with character delimiter
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts>
    [[nodiscard]] constexpr auto operator()(
        const split_result<CharT, Cap, MaxParts>& result,
        CharT delimiter
    ) const noexcept {
        constexpr std::size_t result_cap = Cap * MaxParts + MaxParts;
        basic_fstring<CharT, result_cap> joined;
        
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i > 0 && !joined.full()) {
                joined.push_back(delimiter);
            }
            if (!joined.full()) {
                joined.append(result.parts[i].data(), result.parts[i].size());
            }
        }
        
        return joined;
    }
    
    // Join split_result with string delimiter
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts, std::size_t DelimCap>
    [[nodiscard]] constexpr auto operator()(
        const split_result<CharT, Cap, MaxParts>& result,
        const basic_fstring<CharT, DelimCap>& delimiter
    ) const noexcept {
        constexpr std::size_t result_cap = Cap * MaxParts + DelimCap * MaxParts;
        basic_fstring<CharT, result_cap> joined;
        
        for (std::size_t i = 0; i < result.count; ++i) {
            if (i > 0 && !joined.full()) {
                joined.append(delimiter.data(), delimiter.size());
            }
            if (!joined.full()) {
                joined.append(result.parts[i].data(), result.parts[i].size());
            }
        }
        
        return joined;
    }
    
    // Join with C-string delimiter
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts>
    [[nodiscard]] constexpr auto operator()(
        const split_result<CharT, Cap, MaxParts>& result,
        const CharT* delimiter
    ) const noexcept {
        basic_fstring<CharT, 64> delim(delimiter);
        return (*this)(result, delim);
    }
};

inline constexpr join_fn join;

// ==================== Partition (Split into 2 parts) ====================

struct partition_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT delimiter
    ) const noexcept {
        struct result_t {
            basic_fstring<CharT, Cap> first;
            basic_fstring<CharT, Cap> second;
            bool found = false;
        };
        
        result_t result;
        std::size_t pos = str.find(delimiter);
        
        if (pos != basic_fstring<CharT, Cap>::npos) {
            result.found = true;
            result.first = str.template substr<Cap>(0, pos);
            if (pos + 1 < str.size()) {
                result.second = str.template substr<Cap>(pos + 1);
            }
        } else {
            result.first = str;
        }
        
        return result;
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT delimiter) const noexcept {
        return [delimiter, this](const auto& str) {
            return (*this)(str, delimiter);
        };
    }
};

inline constexpr partition_fn partition;

// ==================== Reverse Split (from right) ====================

struct rsplit_fn {
    template <meta::character CharT, std::size_t Cap, std::size_t MaxParts = 16>
    [[nodiscard]] constexpr auto operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT delimiter
    ) const noexcept {
        split_result<CharT, Cap, MaxParts> result;
        basic_fstring<CharT, Cap> current;
        
        // Build parts in reverse
        for (std::size_t i = str.size(); i > 0 && result.count < MaxParts; --i) {
            CharT ch = str[i - 1];
            
            if (ch == delimiter) {
                if (!current.empty()) {
                    // Reverse current part before storing
                    basic_fstring<CharT, Cap> reversed;
                    for (std::size_t j = current.size(); j > 0; --j) {
                        reversed.push_back(current[j - 1]);
                    }
                    result.parts[result.count++] = reversed;
                    current.clear();
                }
            } else {
                current.push_back(ch);
            }
        }
        
        // Add last part
        if (!current.empty() && result.count < MaxParts) {
            basic_fstring<CharT, Cap> reversed;
            for (std::size_t j = current.size(); j > 0; --j) {
                reversed.push_back(current[j - 1]);
            }
            result.parts[result.count++] = reversed;
        }
        
        // Reverse the entire result array
        for (std::size_t i = 0; i < result.count / 2; ++i) {
            auto temp = result.parts[i];
            result.parts[i] = result.parts[result.count - 1 - i];
            result.parts[result.count - 1 - i] = temp;
        }
        
        return result;
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT delimiter) const noexcept {
        return [delimiter, this](const auto& str) {
            return (*this)(str, delimiter);
        };
    }
};

inline constexpr rsplit_fn rsplit;

} // namespace zuu::str