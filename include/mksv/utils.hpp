#pragma once

#include "types.hpp"

#define _DEFER_1(x, y) x##y
#define _DEFER_2(x, y) _DEFER_1(x, y)
#define _DEFER_3(x) _DEFER_2(x, __COUNTER__)
#define defer(code) auto _DEFER_3(_defer_) = mksv::_defer_func([&]() { code; })

namespace mksv {

template <typename F>
struct _Defer {
    F func;

    _Defer(F func)
        : func{ func } {
    }

    ~_Defer() {
        func();
    }
};

template <typename F>
_Defer<F>
_defer_func(F func) {
    return _Defer<F>(func);
}

constexpr bool
is_digit(const u8 c) {
    return (c >= '0' && c <= '9');
}

template <typename T, T v>
struct integral_constant {
    static constexpr bool value = v;
    using value_type = T;
    using type = integral_constant;
};

template <bool V>
struct bool_constant : public integral_constant<bool, V> {};

struct true_type : public bool_constant<true> {};

struct false_type : public bool_constant<false> {};

template <typename T, typename U>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

} // namespace mksv
