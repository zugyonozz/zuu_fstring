/**
 * @file constant_string_class.hpp
 * @brief Complete string class implementation (merged core + algorithms)
 * @version 1.0
 */

#pragma once

#include "constant_string_iterator.hpp"
#include "constant_string_detail.hpp"
#include <algorithm>
#include <compare>
#include <stdexcept>

namespace zuu {

/**
 * @brief Fixed-size compile-time string class dengan optimasi tinggi
 * 
 * @tparam CharT Character type (char, wchar_t, char8_t, char16_t, char32_t)
 * @tparam N Maximum capacity (excluding null terminator)
 */
template <traits::character CharT = char, std::size_t N = 0>
class string {
public:
    // ========================================================================
    // Type Definitions
    // ========================================================================
    
    using value_type             = CharT;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = CharT&;
    using const_reference        = const CharT&;
    using pointer                = CharT*;
    using const_pointer          = const CharT*;
    using iterator               = string_iterator<CharT, false>;
    using const_iterator         = string_iterator<CharT, true>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = static_cast<size_type>(-1);

private:
    alignas(CharT) CharT data_[N + 1]{};
    size_type len_ = 0;

    constexpr void assign_impl(const CharT* str, size_type count) noexcept {
        len_ = (count < N) ? count : N;
        detail::memcpy_string(data_, str, len_);
        data_[len_] = CharT{};
    }

public:
    // Constructors
    constexpr string() noexcept = default;
    constexpr string(const string&) noexcept = default;
    constexpr string(string&&) noexcept = default;
    ~string() = default;

    constexpr string(const CharT* str) noexcept {
        if (str) assign_impl(str, detail::strlen(str));
    }

    template <size_type M>
    constexpr string(const CharT (&str)[M]) noexcept {
        assign_impl(str, M > 0 ? M - 1 : 0);
    }

    constexpr string(size_type count, CharT ch) noexcept {
        len_ = (count < N) ? count : N;
        detail::memset_string(data_, ch, len_);
        data_[len_] = CharT{};
    }

    template <size_type M>
    constexpr string(const string<CharT, M>& other) noexcept {
        assign_impl(other.data(), other.length());
    }

    // Assignment
    constexpr string& operator=(const string&) noexcept = default;
    constexpr string& operator=(string&&) noexcept = default;

    constexpr string& operator=(const CharT* str) noexcept {
        if (str) assign_impl(str, detail::strlen(str));
        else clear();
        return *this;
    }

    template <size_type M>
    constexpr string& operator=(const string<CharT, M>& other) noexcept {
        assign_impl(other.data(), other.length());
        return *this;
    }

    // Element Access
    [[nodiscard]] constexpr reference at(size_type pos) {
        if (pos >= len_) throw std::out_of_range("zuu::string::at");
        return data_[pos];
    }

    [[nodiscard]] constexpr const_reference at(size_type pos) const {
        if (pos >= len_) throw std::out_of_range("zuu::string::at");
        return data_[pos];
    }

    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept { return data_[pos]; }
    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept { return data_[pos]; }
    [[nodiscard]] constexpr reference front() noexcept { return data_[0]; }
    [[nodiscard]] constexpr const_reference front() const noexcept { return data_[0]; }
    [[nodiscard]] constexpr reference back() noexcept { return data_[len_ > 0 ? len_ - 1 : 0]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return data_[len_ > 0 ? len_ - 1 : 0]; }
    [[nodiscard]] constexpr pointer data() noexcept { return data_; }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return data_; }
    [[nodiscard]] constexpr const_pointer c_str() const noexcept { return data_; }

    // Iterators
    [[nodiscard]] constexpr iterator begin() noexcept { return iterator(data_); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return const_iterator(data_); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return const_iterator(data_); }
    [[nodiscard]] constexpr iterator end() noexcept { return iterator(data_ + len_); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return const_iterator(data_ + len_); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return const_iterator(data_ + len_); }
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    [[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // Capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return len_ == 0; }
    [[nodiscard]] constexpr size_type length() const noexcept { return len_; }
    [[nodiscard]] constexpr size_type size() const noexcept { return len_; }
    [[nodiscard]] constexpr size_type capacity() const noexcept { return N; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }
    [[nodiscard]] constexpr size_type available() const noexcept { return N - len_; }

    // Modifiers
    constexpr void clear() noexcept {
        len_ = 0;
        data_[0] = CharT{};
    }

    constexpr bool push_back(CharT ch) noexcept {
        if (len_ >= N) return false;
        data_[len_++] = ch;
        data_[len_] = CharT{};
        return true;
    }

    constexpr void pop_back() noexcept {
        if (len_ > 0) data_[--len_] = CharT{};
    }

    template <size_type M>
    constexpr size_type append(const string<CharT, M>& other) noexcept {
        return append(other.data(), other.length());
    }

    constexpr size_type append(const CharT* str) noexcept {
        return str ? append(str, detail::strlen(str)) : 0;
    }

    constexpr size_type append(const CharT* str, size_type count) noexcept {
        if (!str || count == 0 || len_ >= N) return 0;
        size_type to_copy = std::min(count, N - len_);
        detail::memcpy_string(data_ + len_, str, to_copy);
        len_ += to_copy;
        data_[len_] = CharT{};
        return to_copy;
    }

    constexpr size_type append(size_type count, CharT ch) noexcept {
        if (count == 0 || len_ >= N) return 0;
        size_type to_add = std::min(count, N - len_);
        detail::memset_string(data_ + len_, ch, to_add);
        len_ += to_add;
        data_[len_] = CharT{};
        return to_add;
    }

    template <size_type M>
    constexpr string& operator+=(const string<CharT, M>& other) noexcept {
        append(other);
        return *this;
    }

    constexpr string& operator+=(const CharT* str) noexcept {
        append(str);
        return *this;
    }

    constexpr string& operator+=(CharT ch) noexcept {
        push_back(ch);
        return *this;
    }

    constexpr string& insert(size_type pos, const CharT* str, size_type count) noexcept {
        if (!str || pos > len_ || count == 0 || len_ >= N) return *this;
        size_type space = N - len_;
        size_type to_insert = std::min(count, space);
        size_type to_move = len_ - pos;
        if (to_move > 0) {
            detail::memmove_string(data_ + pos + to_insert, data_ + pos, to_move);
        }
        detail::memcpy_string(data_ + pos, str, to_insert);
        len_ = std::min(len_ + to_insert, N);
        data_[len_] = CharT{};
        return *this;
    }

    constexpr string& erase(size_type pos = 0, size_type count = npos) noexcept {
        if (pos >= len_) return *this;
        size_type to_erase = std::min(count, len_ - pos);
        size_type new_len = len_ - to_erase;
        size_type to_move = len_ - (pos + to_erase);
        if (to_move > 0) {
            detail::memmove_string(data_ + pos, data_ + pos + to_erase, to_move);
        }
        len_ = new_len;
        data_[len_] = CharT{};
        return *this;
    }

    constexpr string& replace(size_type pos, size_type count, 
                              const CharT* str, size_type str_len) noexcept {
        if (!str || pos > len_) return *this;
        erase(pos, count);
        insert(pos, str, str_len);
        return *this;
    }

    constexpr string& reverse() noexcept {
        for (size_type i = 0; i < len_ / 2; ++i) {
            CharT tmp = data_[i];
            data_[i] = data_[len_ - 1 - i];
            data_[len_ - 1 - i] = tmp;
        }
        return *this;
    }

    constexpr string& to_upper() noexcept requires std::same_as<CharT, char> {
        for (size_type i = 0; i < len_; ++i) {
            data_[i] = detail::to_upper(data_[i]);
        }
        return *this;
    }

    constexpr string& to_lower() noexcept requires std::same_as<CharT, char> {
        for (size_type i = 0; i < len_; ++i) {
            data_[i] = detail::to_lower(data_[i]);
        }
        return *this;
    }

    constexpr string& trim() noexcept requires std::same_as<CharT, char> {
        size_type start = 0;
        while (start < len_ && detail::is_space(data_[start])) ++start;
        size_type end = len_;
        while (end > start && detail::is_space(data_[end - 1])) --end;
        if (start > 0 || end < len_) {
            len_ = end - start;
            if (start > 0) detail::memmove_string(data_, data_ + start, len_);
            data_[len_] = CharT{};
        }
        return *this;
    }

    // Search
    [[nodiscard]] constexpr size_type find(const CharT* str, size_type pos = 0) const noexcept {
        if (!str) return npos;
        return detail::find_substring(data_, len_, str, detail::strlen(str), pos);
    }

    template <size_type M>
    [[nodiscard]] constexpr size_type find(const string<CharT, M>& str, size_type pos = 0) const noexcept {
        return detail::find_substring(data_, len_, str.data(), str.length(), pos);
    }

    [[nodiscard]] constexpr size_type find(CharT ch, size_type pos = 0) const noexcept {
        return detail::find_char(data_, len_, ch, pos);
    }

    [[nodiscard]] constexpr size_type rfind(CharT ch, size_type pos = npos) const noexcept {
        return detail::rfind_char(data_, len_, ch, pos);
    }

    [[nodiscard]] constexpr bool starts_with(const CharT* prefix) const noexcept {
        if (!prefix) return false;
        size_type prefix_len = detail::strlen(prefix);
        if (prefix_len > len_) return false;
        return detail::strcmp(data_, prefix, prefix_len) == 0;
    }

    [[nodiscard]] constexpr bool starts_with(CharT ch) const noexcept {
        return len_ > 0 && data_[0] == ch;
    }

    [[nodiscard]] constexpr bool ends_with(const CharT* suffix) const noexcept {
        if (!suffix) return false;
        size_type suffix_len = detail::strlen(suffix);
        if (suffix_len > len_) return false;
        return detail::strcmp(data_ + (len_ - suffix_len), suffix, suffix_len) == 0;
    }

    [[nodiscard]] constexpr bool ends_with(CharT ch) const noexcept {
        return len_ > 0 && data_[len_ - 1] == ch;
    }

    [[nodiscard]] constexpr bool contains(const CharT* str) const noexcept {
        return find(str) != npos;
    }

    [[nodiscard]] constexpr bool contains(CharT ch) const noexcept {
        return find(ch) != npos;
    }

    [[nodiscard]] constexpr size_type count(CharT ch) const noexcept {
        size_type result = 0;
        for (size_type i = 0; i < len_; ++i) {
            if (data_[i] == ch) ++result;
        }
        return result;
    }

    [[nodiscard]] constexpr string substr(size_type pos = 0, size_type count = npos) const noexcept {
        string result;
        if (pos >= len_) return result;
        size_type actual_count = std::min(count, len_ - pos);
        result.assign_impl(data_ + pos, actual_count);
        return result;
    }

    // Comparison
    template <size_type M>
    [[nodiscard]] constexpr int compare(const string<CharT, M>& other) const noexcept {
        return detail::strcmp_full(data_, len_, other.data(), other.length());
    }

    [[nodiscard]] constexpr int compare(const CharT* str) const noexcept {
        if (!str) return (len_ > 0) ? 1 : 0;
        return detail::strcmp_full(data_, len_, str, detail::strlen(str));
    }

    template <size_type M>
    [[nodiscard]] constexpr bool operator==(const string<CharT, M>& other) const noexcept {
        return len_ == other.length() && compare(other) == 0;
    }

    [[nodiscard]] constexpr bool operator==(const CharT* str) const noexcept {
        return compare(str) == 0;
    }

    template <size_type M>
    [[nodiscard]] constexpr auto operator<=>(const string<CharT, M>& other) const noexcept {
        int cmp = compare(other);
        return (cmp < 0) ? std::strong_ordering::less :
               (cmp > 0) ? std::strong_ordering::greater :
                           std::strong_ordering::equal;
    }

    [[nodiscard]] constexpr auto operator<=>(const CharT* str) const noexcept {
        int cmp = compare(str);
        return (cmp < 0) ? std::strong_ordering::less :
               (cmp > 0) ? std::strong_ordering::greater :
                           std::strong_ordering::equal;
    }

    // Hash
    [[nodiscard]] constexpr std::size_t hash() const noexcept {
        return detail::hash_fnv1a(data_, len_);
    }
};

// Non-member functions
template <traits::character CharT, std::size_t N1, std::size_t N2>
[[nodiscard]] constexpr auto concat(const string<CharT, N1>& a, const string<CharT, N2>& b) noexcept {
    string<CharT, N1 + N2> result;
    result.append(a);
    result.append(b);
    return result;
}

template <traits::character CharT, std::size_t N1, std::size_t N2>
[[nodiscard]] constexpr auto operator+(const string<CharT, N1>& a, const string<CharT, N2>& b) noexcept {
    return concat(a, b);
}

template <traits::character CharT, std::size_t N>
[[nodiscard]] constexpr auto reversed(const string<CharT, N>& str) noexcept {
    string<CharT, N> result = str;
    result.reverse();
    return result;
}

template <std::size_t N>
[[nodiscard]] constexpr auto to_upper(const string<char, N>& str) noexcept {
    string<char, N> result = str;
    result.to_upper();
    return result;
}

template <std::size_t N>
[[nodiscard]] constexpr auto to_lower(const string<char, N>& str) noexcept {
    string<char, N> result = str;
    result.to_lower();
    return result;
}

template <std::size_t N>
[[nodiscard]] constexpr auto trimmed(const string<char, N>& str) noexcept {
    string<char, N> result = str;
    result.trim();
    return result;
}

// Deduction guides
template <traits::character CharT, std::size_t N>
string(const CharT (&)[N]) -> string<CharT, N - 1>;

} // namespace zuu
