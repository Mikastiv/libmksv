#pragma once

#include "math.hpp"
#include "types.hpp"

namespace mksv {
namespace bit {

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

template <typename T>
inline constexpr T
rotate_left(const T n, u8 count) {
    constexpr T nbits = sizeof(T) * 8;
    count %= nbits;

    if (count == 0) return n;

    const T mask = ~((1 << (nbits - count)) - 1);

    const T x = (n & mask) >> (nbits - count);
    const T y = n << count;

    return x | y;
}

} // namespace bit
} // namespace mksv
