#pragma once

#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

constexpr f32 PI = 3.14159265358979323846264338327950288419716939937510f;
constexpr f32 TAU = 2.0f * PI;

template <typename T>
union Vec2 {
    struct {
        T x;
        T y;
    };

    T e[2];
};

template <typename T>
union Vec3 {
    struct {
        T x;
        T y;
        T z;
    };

    T e[3];
};

template <typename T>
union Vec4 {
    struct {
        T x;
        T y;
        T z;
        T w;
    };

    T e[4];
};

template <typename T>
Vec2<T>
operator+(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

template <typename T>
Vec3<T>
operator+(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

template <typename T>
Vec4<T>
operator+(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w,
    };
}

template <typename T>
Vec2<T>
operator-(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

template <typename T>
Vec3<T>
operator-(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

template <typename T>
Vec4<T>
operator-(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w,
    };
}

template <typename T>
Vec2<T>
operator*(const Vec2<T>& a, const T& b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
    };
}

template <typename T>
Vec3<T>
operator*(const Vec3<T>& a, const T& b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b,
    };
}

template <typename T>
Vec4<T>
operator*(const Vec4<T>& a, const T& b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b,
        .w = a.w * b,
    };
}

template <typename T>
Vec2<T>
operator/(const Vec2<T>& a, const T& b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
    };
}

template <typename T>
Vec3<T>
operator/(const Vec3<T>& a, const T& b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
        .z = a.z / b,
    };
}

template <typename T>
Vec4<T>
operator/(const Vec4<T>& a, const T& b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
        .z = a.z / b,
        .w = a.w / b,
    };
}

template <typename T>
struct Mat4 {
    Vec4<T> e[4];
};
