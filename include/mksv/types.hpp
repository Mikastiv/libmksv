#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

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
