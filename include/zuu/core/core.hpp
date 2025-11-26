#pragma once

/**
 * @file zuu/core/core.hpp
 * @brief Core fixed-capacity string storage (minimal interface)
 * @version 3.0.0
 * 
 * Design Philosophy:
 * - Pure storage management (no complex algorithms)
 * - Trivially copyable when possible
 * - Constexpr-friendly
 */

#include "../meta/concepts.hpp"
#include "../meta/traits.hpp"
#include <algorithm>
#include <stdexcept>

namespace zuu {

// ==================== Core Storage Class ====================

template <meta::character CharT, std::size_t Cap>
class basic_fstring {
public:
    // Standard aliases
    using value_type = CharT;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = CharT*;
    using const_pointer = const CharT*;
    using reference = CharT&;
    using const_reference = const CharT&;
    using iterator = CharT*;
    using const_iterator = const CharT*;
    
    static constexpr size_type capacity = Cap;
    static constexpr size_type npos = static_cast<size_type>(-1);

private:
    alignas(CharT) CharT data_[Cap + 1]{}; 
    size_type size_{};

    // Internal helpers
    constexpr void set_null_terminator() noexcept {
        data_[size_] = CharT{};
    }

public:
    // ==================== Construction ====================
    
    constexpr basic_fstring() noexcept = default;
    constexpr basic_fstring(const basic_fstring&) noexcept = default;
    constexpr basic_fstring(basic_fstring&&) noexcept = default;
    
    constexpr basic_fstring& operator=(const basic_fstring&) noexcept = default;
    constexpr basic_fstring& operator=(basic_fstring&&) noexcept = default;
    
    ~basic_fstring() = default;

    // From literal
    template <size_type N>
    constexpr basic_fstring(const CharT (&str)[N]) noexcept {
        size_ = std::min(Cap, N - 1);
        std::copy_n(str, size_, data_);
        set_null_terminator();
    }

    // From pointer + length
    constexpr basic_fstring(const_pointer str, size_type len) noexcept {
        if (str) {
            size_ = std::min(Cap, len);
            std::copy_n(str, size_, data_);
            set_null_terminator();
        }
    }

    // From null-terminated string
    constexpr explicit basic_fstring(const_pointer str) noexcept {
        if (str) {
            size_type len = 0;
            while (str[len] != CharT{} && len < Cap) {
                data_[len] = str[len];
                ++len;
            }
            size_ = len;
            set_null_terminator();
        }
    }

    // Fill constructor
    constexpr basic_fstring(size_type count, CharT ch) noexcept {
        size_ = std::min(Cap, count);
        std::fill_n(data_, size_, ch);
        set_null_terminator();
    }

    // From string_view
    constexpr explicit basic_fstring(std::basic_string_view<CharT> sv) noexcept 
        : basic_fstring(sv.data(), sv.size()) {}

    // From std::string
    constexpr explicit basic_fstring(const std::basic_string<CharT>& str) noexcept 
        : basic_fstring(str.data(), str.size()) {}

    // ==================== Capacity ====================
    
    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] constexpr size_type size() const noexcept { return size_; }
    [[nodiscard]] constexpr size_type length() const noexcept { return size_; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return capacity; }
    [[nodiscard]] constexpr size_type available() const noexcept { return capacity - size_; }
    [[nodiscard]] constexpr bool full() const noexcept { return size_ == capacity; }

    // ==================== Element Access ====================
    
    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept {
        return data_[pos];
    }
    
    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept {
        return data_[pos];
    }

    [[nodiscard]] constexpr const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("fstring::at");
        return data_[pos];
    }

    [[nodiscard]] constexpr reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("fstring::at");
        return data_[pos];
    }

    [[nodiscard]] constexpr reference front() noexcept { return data_[0]; }
    [[nodiscard]] constexpr const_reference front() const noexcept { return data_[0]; }
    [[nodiscard]] constexpr reference back() noexcept { return data_[size_ - 1]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return data_[size_ - 1]; }

    [[nodiscard]] constexpr pointer data() noexcept { return data_; }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return data_; }
    [[nodiscard]] constexpr const_pointer c_str() const noexcept { return data_; }

    // ==================== Iterators ====================
    
    [[nodiscard]] constexpr iterator begin() noexcept { return data_; }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return data_; }
    
    [[nodiscard]] constexpr iterator end() noexcept { return data_ + size_; }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + size_; }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return data_ + size_; }

    // ==================== Modifiers (Core Only) ====================
    
    constexpr void clear() noexcept {
        size_ = 0;
        data_[0] = CharT{};
    }

    constexpr void push_back(CharT ch) noexcept {
        if (!full()) {
            data_[size_++] = ch;
            set_null_terminator();
        }
    }

    constexpr void pop_back() noexcept {
        if (size_ > 0) {
            --size_;
            set_null_terminator();
        }
    }

    constexpr void resize(size_type new_size, CharT ch = CharT{}) noexcept {
        new_size = std::min(new_size, capacity);
        if (new_size > size_) {
            std::fill(data_ + size_, data_ + new_size, ch);
        }
        size_ = new_size;
        set_null_terminator();
    }

    // Append (basic version)
    constexpr basic_fstring& append(const_pointer str, size_type len) noexcept {
        if (str && !full()) {
            len = std::min(len, available());
            std::copy_n(str, len, data_ + size_);
            size_ += len;
            set_null_terminator();
        }
        return *this;
    }

    constexpr basic_fstring& append(CharT ch) noexcept {
        push_back(ch);
        return *this;
    }

    constexpr basic_fstring& append(size_type count, CharT ch) noexcept {
        count = std::min(count, available());
        std::fill_n(data_ + size_, count, ch);
        size_ += count;
        set_null_terminator();
        return *this;
    }

	// ==================== Search Operations ====================

	[[nodiscard]] constexpr size_type find(CharT ch, size_type pos = 0) const noexcept {
        for (size_type i = pos; i < size_; ++i) {
            if (data_[i] == ch) return i;
        }
        return npos;
    }
    
    [[nodiscard]] constexpr size_type find(const_pointer str, size_type pos = 0) const noexcept {
        if (!str) return npos;
        
        size_type str_len = 0;
        while (str[str_len] != CharT{}) ++str_len;
        
        if (str_len == 0) return pos;
        if (pos + str_len > size_) return npos;
        
        for (size_type i = pos; i <= size_ - str_len; ++i) {
            bool match = true;
            for (size_type j = 0; j < str_len; ++j) {
                if (data_[i + j] != str[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        return npos;
    }
    
    [[nodiscard]] constexpr size_type rfind(CharT ch, size_type pos = npos) const noexcept {
        if (size_ == 0) return npos;
        
        size_type search_end = (pos >= size_) ? size_ - 1 : pos;
        for (size_type i = search_end + 1; i > 0; --i) {
            if (data_[i - 1] == ch) return i - 1;
        }
        return npos;
    }
    
    [[nodiscard]] constexpr bool contains(CharT ch) const noexcept {
        return find(ch) != npos;
    }
    
    [[nodiscard]] constexpr bool contains(const_pointer str) const noexcept {
        return find(str) != npos;
    }
    
    [[nodiscard]] constexpr bool starts_with(CharT ch) const noexcept {
        return size_ > 0 && data_[0] == ch;
    }
    
    [[nodiscard]] constexpr bool starts_with(const_pointer str) const noexcept {
        if (!str) return false;
        size_type i = 0;
        while (str[i] != CharT{}) {
            if (i >= size_ || data_[i] != str[i]) return false;
            ++i;
        }
        return true;
    }
    
    [[nodiscard]] constexpr bool ends_with(CharT ch) const noexcept {
        return size_ > 0 && data_[size_ - 1] == ch;
    }
    
    [[nodiscard]] constexpr bool ends_with(const_pointer str) const noexcept {
        if (!str) return false;
        size_type str_len = 0;
        while (str[str_len] != CharT{}) ++str_len;
        
        if (str_len > size_) return false;
        
        for (size_type i = 0; i < str_len; ++i) {
            if (data_[size_ - str_len + i] != str[i]) return false;
        }
        return true;
    }

	// ==================== Substring ====================
    
    template <std::size_t ResultCap = Cap>
    [[nodiscard]] constexpr basic_fstring<CharT, ResultCap> substr(
        size_type pos = 0, 
        size_type count = npos
    ) const noexcept {
        basic_fstring<CharT, ResultCap> result;
        
        if (pos >= size_) return result;
        
        count = std::min(count, size_ - pos);
        result.append(data_ + pos, count);
        
        return result;
    }

    // ==================== Comparison ====================
    
    [[nodiscard]] constexpr auto operator<=>(const basic_fstring&) const noexcept = default;
    
    [[nodiscard]] constexpr bool operator==(std::basic_string_view<CharT> sv) const noexcept {
        return std::basic_string_view<CharT>{data_, size_} == sv;
    }

    // ==================== Conversions ====================
    
    [[nodiscard]] constexpr operator std::basic_string_view<CharT>() const noexcept {
        return {data_, size_};
    }

    [[nodiscard]] constexpr std::basic_string<CharT> to_string() const {
        return {data_, size_};
    }

    // ==================== Concatenation ====================
    
    template <std::size_t N>
    [[nodiscard]] constexpr auto operator+(const basic_fstring<CharT, N>& rhs) const noexcept {
        basic_fstring<CharT, Cap + N> result;
        result.append(data_, size_);
        result.append(rhs.data(), rhs.size());
        return result;
    }

	template <std::size_t N>
    [[nodiscard]] constexpr auto operator+(const CharT (&rhs)[N]) const noexcept {
        basic_fstring<CharT, Cap + N> result;
        result.append(data_, size_);
        result.append(rhs, N);
        return result;
    }

    template <std::size_t N>
    constexpr basic_fstring& operator+=(const basic_fstring<CharT, N>& rhs) noexcept {
        return append(rhs.data(), rhs.size());
    }

	template <std::size_t N>
    constexpr basic_fstring& operator+=(const CharT (&rhs)[N]) noexcept {
        return append(rhs, N);
    }

    constexpr basic_fstring& operator+=(CharT ch) noexcept {
        return append(ch);
    }
};

// ==================== Deduction Guides ====================

template <meta::character CharT, std::size_t N>
basic_fstring(const CharT (&)[N]) -> basic_fstring<CharT, N - 1>;

// ==================== Type Aliases ====================

template <std::size_t Cap> 
using fstring = basic_fstring<char, Cap>;

template <std::size_t Cap> 
using wfstring = basic_fstring<wchar_t, Cap>;

template <std::size_t Cap> 
using u8fstring = basic_fstring<char8_t, Cap>;

template <std::size_t Cap> 
using u16fstring = basic_fstring<char16_t, Cap>;

template <std::size_t Cap> 
using u32fstring = basic_fstring<char32_t, Cap>;

// ==================== Stream Operators ====================

template <meta::character CharT, std::size_t Cap>
std::basic_ostream<CharT>& operator<<(
    std::basic_ostream<CharT>& os, 
    const basic_fstring<CharT, Cap>& str
) {
    return os << str.data();
}

} // namespace zuu