#pragma once

/**
 * @file zuu/str/trim.hpp
 * @brief Trimming algorithms with pipe support
 * @version 3.0.0
 * 
 * Usage:
 *   auto result = trim(str);           // Traditional
 *   auto result = str | trim;           // Piped
 *   auto result = str | trim_left;      // Left only
 */

#include "../core/core.hpp"
#include "pipe.hpp"
#include <string_view>

namespace zuu::str {

// ==================== Helpers ====================

template <meta::character CharT>
constexpr bool is_space(CharT ch) noexcept {
    return ch == CharT(' ') || ch == CharT('\t') || 
           ch == CharT('\n') || ch == CharT('\r') ||
           ch == CharT('\f') || ch == CharT('\v');
}

template <meta::character CharT>
constexpr std::size_t find_first_non_space(std::basic_string_view<CharT> sv) noexcept {
    for (std::size_t i = 0; i < sv.size(); ++i) {
        if (!is_space(sv[i])) return i;
    }
    return sv.size();
}

template <meta::character CharT>
constexpr std::size_t find_last_non_space(std::basic_string_view<CharT> sv) noexcept {
    for (std::size_t i = sv.size(); i > 0; --i) {
        if (!is_space(sv[i - 1])) return i;
    }
    return 0;
}

// ==================== Trim Left ====================

struct trim_left_fn : view_pipe<trim_left_fn> {
    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        const auto start = find_first_non_space(sv);
        
        // Return same capacity fstring
        constexpr std::size_t result_cap = 256; // Default, will be optimized
        basic_fstring<CharT, result_cap> result;
        
        if (start < sv.size()) {
            const auto trimmed = sv.substr(start);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }

    // Overload for fixed-capacity strings (preserve capacity)
    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        std::basic_string_view<CharT> sv{str.data(), str.size()};
        const auto start = find_first_non_space(sv);
        
        basic_fstring<CharT, Cap> result;
        if (start < sv.size()) {
            const auto trimmed = sv.substr(start);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }
};

inline constexpr trim_left_fn trim_left;

// ==================== Trim Right ====================

struct trim_right_fn : view_pipe<trim_right_fn> {
    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        const auto end = find_last_non_space(sv);
        
        constexpr std::size_t result_cap = 256;
        basic_fstring<CharT, result_cap> result;
        
        if (end > 0) {
            const auto trimmed = sv.substr(0, end);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }

    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        std::basic_string_view<CharT> sv{str.data(), str.size()};
        const auto end = find_last_non_space(sv);
        
        basic_fstring<CharT, Cap> result;
        if (end > 0) {
            const auto trimmed = sv.substr(0, end);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }
};

inline constexpr trim_right_fn trim_right;

// ==================== Trim Both ====================

struct trim_fn : view_pipe<trim_fn> {
    template <meta::character CharT>
    constexpr auto apply(std::basic_string_view<CharT> sv) const noexcept {
        const auto start = find_first_non_space(sv);
        const auto end = find_last_non_space(sv);
        
        constexpr std::size_t result_cap = 256;
        basic_fstring<CharT, result_cap> result;
        
        if (start < end) {
            const auto trimmed = sv.substr(start, end - start);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }

    template <meta::character CharT, std::size_t Cap>
    constexpr auto apply(const basic_fstring<CharT, Cap>& str) const noexcept {
        std::basic_string_view<CharT> sv{str.data(), str.size()};
        const auto start = find_first_non_space(sv);
        const auto end = find_last_non_space(sv);
        
        basic_fstring<CharT, Cap> result;
        if (start < end) {
            const auto trimmed = sv.substr(start, end - start);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }
};

inline constexpr trim_fn trim;

// ==================== Custom Predicate Trim ====================

template <typename Pred>
struct trim_if_fn {
    Pred predicate;

    constexpr trim_if_fn(Pred p) : predicate{std::move(p)} {}

    template <meta::character CharT, std::size_t Cap>
    constexpr auto operator()(const basic_fstring<CharT, Cap>& str) const noexcept {
        std::basic_string_view<CharT> sv{str.data(), str.size()};
        
        std::size_t start = 0;
        while (start < sv.size() && predicate(sv[start])) ++start;
        
        std::size_t end = sv.size();
        while (end > start && predicate(sv[end - 1])) --end;
        
        basic_fstring<CharT, Cap> result;
        if (start < end) {
            const auto trimmed = sv.substr(start, end - start);
            result.append(trimmed.data(), trimmed.size());
        }
        
        return result;
    }

    template <meta::string_like Str>
    friend constexpr auto operator|(Str&& str, const trim_if_fn& t) {
        return t(std::forward<Str>(str));
    }
};

// Factory function for custom trim
template <typename Pred>
constexpr auto trim_if(Pred&& pred) {
    return trim_if_fn{std::forward<Pred>(pred)};
}

} // namespace zuu::str