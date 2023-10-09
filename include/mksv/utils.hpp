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

inline constexpr bool
is_digit(const u8 c) {
    return (c >= '0' && c <= '9');
}

inline constexpr u32
clz(u32 x) {
    if (x == 0) return 32;

    u32 n = 31;

    u32 y = x << 16;
    if (y != 0) {
        n -= 16;
        x = y;
    }

    y = x << 8;
    if (y != 0) {
        n -= 8;
        x = y;
    }

    y = x << 4;
    if (y != 0) {
        n -= 4;
        x = y;
    }

    y = x << 2;
    if (y != 0) {
        n -= 2;
        x = y;
    }

    y = x << 1;
    if (y != 0) n -= 1;

    return n;
}

} // namespace mksv
