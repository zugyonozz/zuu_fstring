#pragma once

/**
 * @file zuu/str/pipe.hpp
 * @brief Piping infrastructure for functional composition
 * @version 3.0.0
 * 
 * Enables: str | trim | to_upper | split(',')
 */

#include "../meta/concepts.hpp"
#include "../meta/traits.hpp"
#include <tuple>
#include <utility>

namespace zuu::str {

// ==================== Pipe Adaptor Base ====================

/**
 * @brief CRTP base for creating pipeable algorithms
 * 
 * Usage:
 * struct my_algo : pipe_adaptor<my_algo> {
 *     template <meta::string_like Str>
 *     constexpr auto operator()(Str&& str) const { ... }
 * };
 * inline constexpr my_algo my_transform;
 */
template <typename Derived>
struct pipe_adaptor {
    // Direct call: algo(str)
    template <meta::string_like Str>
    constexpr auto operator()(Str&& str) const {
        // Use helper to delay instantiation until Derived is complete
        return pipe_impl(*this, std::forward<Str>(str));
    }

private:
    // Helper function instantiated only when called (Derived is complete by then)
    template <typename Self, meta::string_like Str>
    static constexpr auto pipe_impl(const Self& self, Str&& str) {
        return static_cast<const Derived&>(self).apply(std::forward<Str>(str));
    }

public:
    // Pipe operator: str | algo
    template <meta::string_like Str>
    friend constexpr auto operator|(Str&& str, const pipe_adaptor& algo) {
        return static_cast<const Derived&>(algo)(std::forward<Str>(str));
    }
};

// ==================== Closure Object (for parameterized algos) ====================

/**
 * @brief Wrapper for algorithms that need parameters
 * 
 * Example: split(',') returns a closure that captures ','
 */
template <typename Fn, typename... Args>
struct closure {
    Fn fn;
    std::tuple<Args...> args;

    constexpr closure(Fn f, Args... a) 
        : fn{std::move(f)}, args{std::move(a)...} {}

    template <meta::string_like Str>
    constexpr auto operator()(Str&& str) const {
        return std::apply(
            [&](auto&&... captured) {
                return fn(std::forward<Str>(str), std::forward<decltype(captured)>(captured)...);
            },
            args
        );
    }

    template <meta::string_like Str>
    friend constexpr auto operator|(Str&& str, const closure& c) {
        return c(std::forward<Str>(str));
    }
};

// Helper to create closures
template <typename Fn, typename... Args>
constexpr auto make_closure(Fn&& fn, Args&&... args) {
    return closure<std::decay_t<Fn>, std::decay_t<Args>...>{
        std::forward<Fn>(fn), 
        std::forward<Args>(args)...
    };
}

// ==================== View Adaptor ====================

/**
 * @brief Converts string to string_view for zero-copy algorithms
 */
template <typename Derived>
struct view_pipe : pipe_adaptor<Derived> {
    template <meta::string_like Str>
    constexpr auto apply(Str&& str) const {
        using char_type = meta::char_type_of_t<Str>;
        
        if constexpr (requires { std::basic_string_view{str}; }) {
            return apply_impl(std::basic_string_view<char_type>{str});
        } else if constexpr (requires { str.data(); str.size(); }) {
            return apply_impl(std::basic_string_view<char_type>{str.data(), str.size()});
        } else {
            static_assert(!sizeof(Str), "String type must be convertible to string_view");
        }
    }

private:
    template <meta::character CharT>
    constexpr auto apply_impl(std::basic_string_view<CharT> sv) const {
        return static_cast<const Derived&>(*this).apply(sv);
    }
};

// ==================== Compose Multiple Pipes ====================

/**
 * @brief Compose two pipeable operations
 * 
 * auto pipeline = trim | to_upper;
 * result = str | pipeline;
 */
template <typename Fn1, typename Fn2>
struct composed_pipe {
    Fn1 first;
    Fn2 second;

    constexpr composed_pipe(Fn1 f1, Fn2 f2) 
        : first{std::move(f1)}, second{std::move(f2)} {}

    template <meta::string_like Str>
    constexpr auto operator()(Str&& str) const {
        return second(first(std::forward<Str>(str)));
    }

    template <meta::string_like Str>
    friend constexpr auto operator|(Str&& str, const composed_pipe& cp) {
        return cp(std::forward<Str>(str));
    }
};

// Composition operator for pipes
template <typename Fn1, typename Fn2>
requires requires(Fn1 f1, Fn2 f2) {
    { f1 } -> std::convertible_to<Fn1>;
    { f2 } -> std::convertible_to<Fn2>;
}
constexpr auto operator|(Fn1 f1, Fn2 f2) {
    return composed_pipe{std::move(f1), std::move(f2)};
}

} // namespace zuu::str