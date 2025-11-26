#pragma once

/**
 * @file zuu/str/case.hpp
 * @brief Case conversion algorithms with pipe support
 * @version 3.0.0
 * 
 * Usage:
 *   auto result = to_upper(str);
 *   auto result = str | to_lower | reverse;  // Composable!
 */

#include "../core/core.hpp"
#include "pipe.hpp"

namespace zuu::str {

// ==================== Helpers ====================

template <meta::character CharT>
constexpr CharT char_to_lower(CharT ch) noexcept {
    if (ch >= CharT('A') && ch <= CharT('Z')) {
        return ch + (CharT('a') - CharT('A'));
    }
    return ch;
}

template <meta::character CharT>
constexpr CharT char_to_upper(CharT ch) noexcept {
    if (ch >= CharT('a') && ch <= CharT('z')) {
        return ch - (CharT('a') - CharT('A'));
    }
    return ch;
}

template <meta::character CharT>
constexpr bool is_alpha(CharT ch) noexcept {
    return (ch >= CharT('a') && ch <= CharT('z')) ||
           (ch >= CharT('A') && ch <= CharT('Z'));
}

template <meta::character CharT>
constexpr bool is_whitespace(CharT ch) noexcept {
    return ch == CharT(' ') || ch == CharT('\t') || 
           ch == CharT('\n') || ch == CharT('\r');
}

// ==================== To Lower ====================

struct to_lower_fn : pipe_adaptor<to_lower_fn> {
    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        basic_fstring<CharT, Cap> result;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            result.push_back(char_to_lower(str[i]));
        }
        
        return result;
    }

    // Generic string_view version
    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        constexpr std::size_t default_cap = 256;
        basic_fstring<CharT, default_cap> result;
        
        for (std::size_t i = 0; i < sv.size() && !result.full(); ++i) {
            result.push_back(char_to_lower(sv[i]));
        }
        
        return result;
    }
};

inline constexpr to_lower_fn to_lower;

// ==================== To Upper ====================

struct to_upper_fn : pipe_adaptor<to_upper_fn> {
    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        basic_fstring<CharT, Cap> result;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            result.push_back(char_to_upper(str[i]));
        }
        
        return result;
    }

    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        constexpr std::size_t default_cap = 256;
        basic_fstring<CharT, default_cap> result;
        
        for (std::size_t i = 0; i < sv.size() && !result.full(); ++i) {
            result.push_back(char_to_upper(sv[i]));
        }
        
        return result;
    }
};

inline constexpr to_upper_fn to_upper;

// ==================== To Title Case ====================

struct to_title_fn : pipe_adaptor<to_title_fn> {
    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        basic_fstring<CharT, Cap> result;
        bool capitalize_next = true;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            CharT ch = str[i];
            
            if (is_whitespace(ch)) {
                capitalize_next = true;
                result.push_back(ch);
            } else if (is_alpha(ch)) {
                result.push_back(capitalize_next ? char_to_upper(ch) : char_to_lower(ch));
                capitalize_next = false;
            } else {
                result.push_back(ch);
                capitalize_next = false;
            }
        }
        
        return result;
    }

    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        constexpr std::size_t default_cap = 256;
        basic_fstring<CharT, default_cap> result;
        bool capitalize_next = true;
        
        for (std::size_t i = 0; i < sv.size() && !result.full(); ++i) {
            CharT ch = sv[i];
            
            if (is_whitespace(ch)) {
                capitalize_next = true;
                result.push_back(ch);
            } else if (is_alpha(ch)) {
                result.push_back(capitalize_next ? char_to_upper(ch) : char_to_lower(ch));
                capitalize_next = false;
            } else {
                result.push_back(ch);
                capitalize_next = false;
            }
        }
        
        return result;
    }
};

inline constexpr to_title_fn to_title;

// ==================== Toggle Case ====================

struct toggle_case_fn : pipe_adaptor<toggle_case_fn> {
    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        basic_fstring<CharT, Cap> result;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            CharT ch = str[i];
            if (ch >= CharT('a') && ch <= CharT('z')) {
                result.push_back(char_to_upper(ch));
            } else if (ch >= CharT('A') && ch <= CharT('Z')) {
                result.push_back(char_to_lower(ch));
            } else {
                result.push_back(ch);
            }
        }
        
        return result;
    }
};

inline constexpr toggle_case_fn toggle_case;

// ==================== Case-Insensitive Comparison ====================

struct equals_ignore_case_fn {
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& lhs,
        const basic_fstring<CharT, Cap2>& rhs
    ) const noexcept {
        if (lhs.size() != rhs.size()) return false;
        
        for (std::size_t i = 0; i < lhs.size(); ++i) {
            if (char_to_lower(lhs[i]) != char_to_lower(rhs[i])) {
                return false;
            }
        }
        
        return true;
    }
};

inline constexpr equals_ignore_case_fn equals_ignore_case;

} // namespace zuu::str