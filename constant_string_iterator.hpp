/**
 * @file constant_string_iterator.hpp
 * @brief Random access iterator untuk constant string
 * @version 1.0
 */

#pragma once

#include "constant_string_traits.hpp"
#include <iterator>

namespace zuu {

/**
 * @brief Random access iterator untuk zuu::string
 * 
 * @tparam CharT Character type
 * @tparam IsConst Whether this is a const iterator
 * 
 * Iterator ini fully compatible dengan STL algorithms dan ranges
 */
template <traits::character CharT, bool IsConst>
class string_iterator {
public:
    // Iterator traits
    using iterator_category = std::random_access_iterator_tag;
    using iterator_concept  = std::contiguous_iterator_tag;
    using value_type        = CharT;
    using difference_type   = std::ptrdiff_t;
    using pointer           = std::conditional_t<IsConst, const CharT*, CharT*>;
    using reference         = std::conditional_t<IsConst, const CharT&, CharT&>;

private:
    pointer ptr_ = nullptr;

public:
    // ========================================================================
    // Constructors
    // ========================================================================
    
    constexpr string_iterator() noexcept = default;
    
    constexpr explicit string_iterator(pointer p) noexcept : ptr_(p) {}
    
    // Konversi dari non-const ke const iterator
    constexpr string_iterator(const string_iterator<CharT, false>& other) noexcept
        requires IsConst 
        : ptr_(other.base()) {}

    // ========================================================================
    // Element Access
    // ========================================================================
    
    [[nodiscard]] constexpr pointer base() const noexcept { 
        return ptr_; 
    }
    
    [[nodiscard]] constexpr reference operator*() const noexcept { 
        return *ptr_; 
    }
    
    [[nodiscard]] constexpr pointer operator->() const noexcept { 
        return ptr_; 
    }
    
    [[nodiscard]] constexpr reference operator[](difference_type n) const noexcept { 
        return ptr_[n]; 
    }

    // ========================================================================
    // Increment/Decrement
    // ========================================================================
    
    constexpr string_iterator& operator++() noexcept { 
        ++ptr_; 
        return *this; 
    }
    
    constexpr string_iterator& operator--() noexcept { 
        --ptr_; 
        return *this; 
    }
    
    constexpr string_iterator operator++(int) noexcept { 
        string_iterator tmp = *this; 
        ++ptr_; 
        return tmp; 
    }
    
    constexpr string_iterator operator--(int) noexcept { 
        string_iterator tmp = *this; 
        --ptr_; 
        return tmp; 
    }

    // ========================================================================
    // Arithmetic Operators
    // ========================================================================
    
    constexpr string_iterator& operator+=(difference_type n) noexcept { 
        ptr_ += n; 
        return *this; 
    }
    
    constexpr string_iterator& operator-=(difference_type n) noexcept { 
        ptr_ -= n; 
        return *this; 
    }

    [[nodiscard]] constexpr string_iterator operator+(difference_type n) const noexcept {
        return string_iterator(ptr_ + n);
    }
    
    [[nodiscard]] constexpr string_iterator operator-(difference_type n) const noexcept {
        return string_iterator(ptr_ - n);
    }
    
    [[nodiscard]] constexpr difference_type operator-(
        const string_iterator& other) const noexcept {
        return ptr_ - other.ptr_;
    }

    // ========================================================================
    // Comparison Operators (C++20 spaceship)
    // ========================================================================
    
    [[nodiscard]] constexpr bool operator==(const string_iterator& other) const noexcept {
        return ptr_ == other.ptr_;
    }
    
    [[nodiscard]] constexpr auto operator<=>(const string_iterator& other) const noexcept {
        return ptr_ <=> other.ptr_;
    }
};

// ============================================================================
// Non-member Arithmetic Operators
// ============================================================================

template <traits::character CharT, bool IsConst>
[[nodiscard]] constexpr string_iterator<CharT, IsConst> operator+(
    typename string_iterator<CharT, IsConst>::difference_type n,
    const string_iterator<CharT, IsConst>& it) noexcept {
    return it + n;
}

} // namespace zuu
