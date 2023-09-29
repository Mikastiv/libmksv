#pragma once

#include "mat.hpp"
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

template <typename T>
constexpr Mat4<T>
scaling(const Vec3<T> s) {
    return {
        Vec4<T>(s.x, 0, 0, 0),
        Vec4<T>(0, s.y, 0, 0),
        Vec4<T>(0, 0, s.z, 0),
        Vec4<T>(0, 0, 0, 1),
    };
}

template <typename T>
constexpr Mat4<T>
translation(const Vec3<T> t) {
    return {
        Vec4<T>(1, 0, 0, 0),
        Vec4<T>(0, 1, 0, 0),
        Vec4<T>(0, 0, 1, 0),
        Vec4<T>(t.x, t.y, t.z, 1),
    };
}

// TODO: set as constexpr
template <typename T>
Mat4<T>
rotation(const T angle, const Vec3<T> a) {
    const T ca = cos(angle);
    const T ca1 = 1 - cos(angle);
    const T sin_a = sin(angle);
    const T sax = a.x * sin_a;
    const T say = a.y * sin_a;
    const T saz = a.z * sin_a;
    const T xy = a.x * a.y;
    const T xz = a.x * a.z;
    const T yz = a.y * a.z;
    const T xx = a.x * a.x;
    const T yy = a.y * a.y;
    const T zz = a.z * a.z;

    // clang-format off
    return {
        ca + xx * ca1,  xy * ca1 + saz, xz * ca1 - say, 0,
        xy * ca1 - saz, ca + yy * ca1,  yz * ca1 + sax, 0,
        xz * ca1 + say, yz * ca1 - sax, ca + zz * ca1,  0,
        0,              0,             0,               1,
    };
    // clang-format on
}

template <typename T>
constexpr Mat4<T>
scale(const Mat4<T>& m, const Vec3<T> s) {
    return m * scaling(s);
}

template <typename T>
constexpr Mat4<T>
translate(const Mat4<T>& m, const Vec3<T> t) {
    return m * translation(t);
}

// TODO: set as constexpr
template <typename T>
Mat4<T>
rotate(const Mat4<T>& m, const T angle, const Vec3<T> axis) {
    return m * rotation(angle, axis);
}

} // namespace math
} // namespace mksv
