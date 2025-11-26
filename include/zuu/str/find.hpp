#pragma once

/**
 * @file zuu/str/find.hpp
 * @brief Search and find operations with pipe support
 * @version 3.0.0
 * 
 * Usage:
 *   bool has = contains(str, 'x');
 *   bool has = str | contains('x');
 *   bool starts = starts_with(str, "prefix");
 */

#include "../core/core.hpp"
#include "pipe.hpp"

namespace zuu::str {

// ==================== Contains (Character) ====================

struct contains_char_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str, 
        CharT ch
    ) const noexcept {
        return str.find(ch) != basic_fstring<CharT, Cap>::npos;
    }
    
    // Factory for piping: str | contains('x')
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch) const noexcept {
        return [ch, this](const auto& str) {
            return (*this)(str, ch);
        };
    }
};

// ==================== Contains (String) ====================

struct contains_str_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* substr
    ) const noexcept {
        return str.find(substr) != basic_fstring<CharT, Cap>::npos;
    }
    
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& substr
    ) const noexcept {
        return str.find(substr.data()) != basic_fstring<CharT, Cap1>::npos;
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* substr) const noexcept {
        return [substr, this](const auto& str) {
            return (*this)(str, substr);
        };
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, Cap>& substr) const noexcept {
        return [substr, this](const auto& str) {
            return (*this)(str, substr);
        };
    }
};

// Combined contains function
struct contains_fn {
    // Character overload
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch
    ) const noexcept {
        return contains_char_fn{}(str, ch);
    }
    
    // String overload
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* substr
    ) const noexcept {
        return contains_str_fn{}(str, substr);
    }
    
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& substr
    ) const noexcept {
        return contains_str_fn{}(str, substr);
    }
    
    // Factory for piping (character)
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch) const noexcept {
        return [ch, this](const auto& str) {
            return (*this)(str, ch);
        };
    }
    
    // Factory for piping (string)
    template <typename T>
    requires (!std::same_as<T, typename std::decay_t<T>::value_type>)
    [[nodiscard]] constexpr auto operator()(T&& substr) const noexcept {
        return [substr = std::forward<T>(substr), this](const auto& str) {
            return (*this)(str, substr);
        };
    }
};

inline constexpr contains_fn contains;

// ==================== Starts With ====================

struct starts_with_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* prefix
    ) const noexcept {
        return str.starts_with(prefix);
    }
    
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& prefix
    ) const noexcept {
        return str.starts_with(prefix.data());
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch
    ) const noexcept {
        return str.starts_with(ch);
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* prefix) const noexcept {
        return [prefix, this](const auto& str) {
            return (*this)(str, prefix);
        };
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, Cap>& prefix) const noexcept {
        return [prefix, this](const auto& str) {
            return (*this)(str, prefix);
        };
    }
    
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch) const noexcept {
        return [ch, this](const auto& str) {
            return (*this)(str, ch);
        };
    }
};

inline constexpr starts_with_fn starts_with;

// ==================== Ends With ====================

struct ends_with_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* suffix
    ) const noexcept {
        return str.ends_with(suffix);
    }
    
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& suffix
    ) const noexcept {
        return str.ends_with(suffix.data());
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch
    ) const noexcept {
        return str.ends_with(ch);
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* suffix) const noexcept {
        return [suffix, this](const auto& str) {
            return (*this)(str, suffix);
        };
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, Cap>& suffix) const noexcept {
        return [suffix, this](const auto& str) {
            return (*this)(str, suffix);
        };
    }
    
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch) const noexcept {
        return [ch, this](const auto& str) {
            return (*this)(str, ch);
        };
    }
};

inline constexpr ends_with_fn ends_with;

// ==================== Find (Return Position) ====================

struct find_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch,
        std::size_t pos = 0
    ) const noexcept {
        return str.find(ch, pos);
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* substr,
        std::size_t pos = 0
    ) const noexcept {
        return str.find(substr, pos);
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch, std::size_t pos = 0) const noexcept {
        return [ch, pos, this](const auto& str) {
            return (*this)(str, ch, pos);
        };
    }
    
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* substr, std::size_t pos = 0) const noexcept {
        return [substr, pos, this](const auto& str) {
            return (*this)(str, substr, pos);
        };
    }
};

inline constexpr find_fn find;

// ==================== Reverse Find ====================

struct rfind_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch,
        std::size_t pos = basic_fstring<CharT, Cap>::npos
    ) const noexcept {
        return str.rfind(ch, pos);
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(
        CharT ch, 
        std::size_t pos = std::size_t(-1)
    ) const noexcept {
        return [ch, pos, this](const auto& str) {
            return (*this)(str, ch, pos);
        };
    }
};

inline constexpr rfind_fn rfind;

// ==================== Count Occurrences ====================

struct count_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        CharT ch
    ) const noexcept {
        std::size_t cnt = 0;
        for (std::size_t i = 0; i < str.size(); ++i) {
            if (str[i] == ch) ++cnt;
        }
        return cnt;
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* substr
    ) const noexcept {
        if (substr == nullptr) return 0;
        
        std::size_t substr_len = 0;
        while (substr[substr_len] != CharT{}) ++substr_len;
        
        if (substr_len == 0) return 0;
        
        std::size_t cnt = 0;
        std::size_t pos = 0;
        
        while (pos < str.size()) {
            std::size_t found = str.find(substr, pos);
            if (found == basic_fstring<CharT, Cap>::npos) break;
            ++cnt;
            pos = found + substr_len;
        }
        
        return cnt;
    }
    
    // Factory for piping
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(CharT ch) const noexcept {
        return [ch, this](const auto& str) {
            return (*this)(str, ch);
        };
    }
    
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* substr) const noexcept {
        return [substr, this](const auto& str) {
            return (*this)(str, substr);
        };
    }
};

inline constexpr count_fn count;

// ==================== Find First Of (any character from set) ====================

struct find_first_of_fn {
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& charset
    ) const noexcept {
        for (std::size_t i = 0; i < str.size(); ++i) {
            for (std::size_t j = 0; j < charset.size(); ++j) {
                if (str[i] == charset[j]) {
                    return i;
                }
            }
        }
        return basic_fstring<CharT, Cap1>::npos;
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* charset
    ) const noexcept {
        if (charset == nullptr) return basic_fstring<CharT, Cap>::npos;
        
        for (std::size_t i = 0; i < str.size(); ++i) {
            for (std::size_t j = 0; charset[j] != CharT{}; ++j) {
                if (str[i] == charset[j]) {
                    return i;
                }
            }
        }
        return basic_fstring<CharT, Cap>::npos;
    }
    
    // Factory for piping
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, Cap>& charset) const noexcept {
        return [charset, this](const auto& str) {
            return (*this)(str, charset);
        };
    }
    
    template <meta::character CharT>
    [[nodiscard]] constexpr auto operator()(const CharT* charset) const noexcept {
        return [charset, this](const auto& str) {
            return (*this)(str, charset);
        };
    }
};

inline constexpr find_first_of_fn find_first_of;

// ==================== Find Last Of ====================

struct find_last_of_fn {
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& charset
    ) const noexcept {
        for (std::size_t i = str.size(); i > 0; --i) {
            for (std::size_t j = 0; j < charset.size(); ++j) {
                if (str[i - 1] == charset[j]) {
                    return i - 1;
                }
            }
        }
        return basic_fstring<CharT, Cap1>::npos;
    }
    
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* charset
    ) const noexcept {
        if (charset == nullptr) return basic_fstring<CharT, Cap>::npos;
        
        for (std::size_t i = str.size(); i > 0; --i) {
            for (std::size_t j = 0; charset[j] != CharT{}; ++j) {
                if (str[i - 1] == charset[j]) {
                    return i - 1;
                }
            }
        }
        return basic_fstring<CharT, Cap>::npos;
    }
    
    // Factory for piping
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr auto operator()(const basic_fstring<CharT, Cap>& charset) const noexcept {
        return [charset, this](const auto& str) {
            return (*this)(str, charset);
        };
    }
};

inline constexpr find_last_of_fn find_last_of;

// ==================== Find First Not Of ====================

struct find_first_not_of_fn {
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr std::size_t operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& charset
    ) const noexcept {
        for (std::size_t i = 0; i < str.size(); ++i) {
            bool found = false;
            for (std::size_t j = 0; j < charset.size(); ++j) {
                if (str[i] == charset[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) return i;
        }
        return basic_fstring<CharT, Cap1>::npos;
    }
};

inline constexpr find_first_not_of_fn find_first_not_of;

// ==================== Contains Any (Check if any char from set exists) ====================

struct contains_any_fn {
    template <meta::character CharT, std::size_t Cap>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap>& str,
        const CharT* charset
    ) const noexcept {
        return find_first_of_fn{}(str, charset) != basic_fstring<CharT, Cap>::npos;
    }
    
    template <meta::character CharT, std::size_t Cap1, std::size_t Cap2>
    [[nodiscard]] constexpr bool operator()(
        const basic_fstring<CharT, Cap1>& str,
        const basic_fstring<CharT, Cap2>& charset
    ) const noexcept {
        return find_first_of_fn{}(str, charset) != basic_fstring<CharT, Cap1>::npos;
    }
    
    // Factory for piping
    template <typename T>
    [[nodiscard]] constexpr auto operator()(T&& charset) const noexcept {
        return [charset = std::forward<T>(charset), this](const auto& str) {
            return (*this)(str, charset);
        };
    }
};

inline constexpr contains_any_fn contains_any;

} // namespace zuu::str