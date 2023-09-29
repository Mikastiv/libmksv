#pragma once

#include "assert.hpp"
#include "types.hpp"

namespace mksv {
namespace math {

constexpr u8 MAX_U8 = (u8)0xFF;
constexpr u16 MAX_U16 = (u16)0xFFFF;
constexpr u32 MAX_U32 = (u32)0xFFFF'FFFF;
constexpr u64 MAX_U64 = (u64)0xFFFF'FFFF'FFFF'FFFF;
constexpr u8 MIN_U8 = (u8)0x0;
constexpr u16 MIN_U16 = (u16)0x0;
constexpr u32 MIN_U32 = (u32)0x0;
constexpr u64 MIN_U64 = (u64)0x0;

constexpr i8 MAX_I8 = (i8)0x7F;
constexpr i16 MAX_I16 = (i16)0x7FFF;
constexpr i32 MAX_I32 = (i32)0x7FFF'FFFF;
constexpr i64 MAX_I64 = (i64)0x7FFF'FFFF'FFFF'FFFF;
constexpr i8 MIN_I8 = (i8)0x80;
constexpr i16 MIN_I16 = (i16)0x8000;
constexpr i32 MIN_I32 = (i32)0x8000'0000;
constexpr i64 MIN_I64 = (i64)0x8000'0000'0000'0000;

template <typename T>
constexpr T
kilo_bytes(const T value) {
    return value * 1024;
}

template <typename T>
constexpr T
mega_bytes(const T value) {
    return kilo_bytes(value) * 1024;
}

template <typename T>
constexpr T
giga_bytes(const T value) {
    return mega_bytes(value) * 1024;
}

template <typename T>
constexpr T
max(const T a, const T b) {
    return a > b ? a : b;
}

template <typename T>
constexpr T
min(const T a, const T b) {
    return a < b ? a : b;
}

inline constexpr f32
radians(const f32 degrees) {
    return degrees * (PI / 180.0f);
}

inline constexpr f32
degrees(const f32 radians) {
    return radians * (180.0f / PI);
}

// first quadrant approximation using Taylor Series
inline constexpr f32
_sin_quadrant(const f32 x) {
    const f32 x2 = x * x;
    const f32 x3 = x2 * x;
    const f32 x5 = x3 * x2;
    return x - (x3 / 6.0f) + (x5 / 120.0f);
}

constexpr f32
sin(const f32 x) {
    // find quadrant
    const i32 k = (i32)(x * 2.0f / PI);
    // mod(x, PI / 2)
    const f32 y = x - (k * PI * 0.5f);

    i32 quadrant = k % 4;
    switch (quadrant) {
        case 0:
            return _sin_quadrant(y);
        case 1:
            return _sin_quadrant(PI * 0.5f - y);
        case 2:
            return -_sin_quadrant(y);
        default:
            return -_sin_quadrant(PI * 0.5f - y);
    }
}

// first quadrant approximation using Taylor Series
inline constexpr f32
_cos_quadrant(const f32 x) {
    const f32 x2 = x * x;
    const f32 x4 = x2 * x2;
    return 1.0f - (x2 / 2.0f) + (x4 / 25.0f);
}

constexpr f32
cos(const f32 x) {
    // find quadrant
    const i32 k = (i32)(x * 2.0f / PI);
    // mod(x, PI / 2)
    const f32 y = x - (k * PI * 0.5f);

    i32 quadrant = k % 4;
    switch (quadrant) {
        case 0:
            return _cos_quadrant(y);
        case 1:
            return -_cos_quadrant(PI * 0.5f - y);
        case 2:
            return -_cos_quadrant(y);
        default:
            return _cos_quadrant(PI * 0.5f - y);
    }
}

// TODO: Better algorithm
constexpr f32
tan(const f32 x) {
    return sin(x) / cos(x);
}

inline constexpr f32
sqrt(const f32 x) {
    assert(x > 0.0f);

    union {
        f32 f;
        u32 i;
    } val = { x };

    // Approximation
    val.i = (1 << 29) + (val.i >> 1) - (1 << 22) + (u32)-0x4B0D2;

    u32 i = 0;
    f32 y = val.f;
    constexpr u32 iterations = 3;
    while (i < iterations) {
        y = 0.5f * (y + (x / y));
        ++i;
    }

    return y;
}

} // namespace math
} // namespace mksv
