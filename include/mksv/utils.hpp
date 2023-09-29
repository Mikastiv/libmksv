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

} // namespace mksv
