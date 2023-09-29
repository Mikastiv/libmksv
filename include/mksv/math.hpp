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
scale(const Mat4<T>& m, const Vec3<T> s) {
    Mat4f out;
    out[0] = m[0] * s[0];
    out[1] = m[1] * s[1];
    out[2] = m[2] * s[2];
    out[3] = m[3];
    return out;
}

template <typename T>
constexpr Mat4<T>
scale(const Mat4<T>& m, const T s) {
    return scale(m, Vec3<T>(s, s, s));
}

template <typename T>
constexpr Mat4<T>
translate(const Mat4<T>& m, const Vec3<T> t) {
    Mat4f out = m;
    out[3] = m[0] * t[0] + m[1] * t[1] + m[2] * t[2] + m[3];
    return out;
}

// TODO: set as constexpr
template <typename T>
Mat4<T>
rotate(const Mat4<T>& m, const T angle, Vec3<T> axis) {
    const T c = cos(angle);
    const T s = sin(angle);
    axis = axis.unit();
    Vec3f t = { axis * ((T)1 - c) };

    Mat4f r;
    r[0][0] = c + t[0] * axis[0];
    r[0][1] = t[0] * axis[1] + s * axis[2];
    r[0][2] = t[0] * axis[2] - s * axis[1];
    r[1][0] = t[1] * axis[0] - s * axis[2];
    r[1][1] = c + t[1] * axis[1];
    r[1][2] = t[1] * axis[2] + s * axis[0];
    r[2][0] = t[2] * axis[0] + s * axis[1];
    r[2][1] = t[2] * axis[1] - s * axis[0];
    r[2][2] = c + t[2] * axis[2];

    Mat4f out;
    out[0] = m[0] * r[0][0] + m[1] * r[0][1] + m[2] * r[0][2];
    out[1] = m[0] * r[1][0] + m[1] * r[1][1] + m[2] * r[1][2];
    out[2] = m[0] * r[2][0] + m[1] * r[2][1] + m[2] * r[2][2];
    out[3] = m[3];
    return out;
}

} // namespace math
} // namespace mksv
