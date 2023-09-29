#pragma once

#include "assert.hpp"

#include <stdint.h>

// TODO: remove math.h
#include <cmath>

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
        T x = 0;
        T y = 0;
    };

    T e[2];

    constexpr Vec2() {
    }

    constexpr Vec2(const T v)
        : x{ v },
          y{ v } {
    }

    constexpr Vec2(const T x, const T y)
        : x{ x },
          y{ y } {
    }

    T&
    operator[](const u64 idx) {
        assert(idx < 2);
        return e[idx];
    }

    const T&
    operator[](const u64 idx) const {
        assert(idx < 2);
        return e[idx];
    }

    constexpr T
    length() const {
        return sqrt((x * x) + (y * y));
    }

    constexpr T
    unit() const {
        const T len = length();
        return {
            .x = x / len,
            .y = y / len,
        };
    }

    constexpr T
    dot(const Vec2& other) const {
        return (x * other.x) + (y * other.y);
    }
};

template <typename T>
union Vec3 {
    struct {
        T x = 0;
        T y = 0;
        T z = 0;
    };

    T e[3];

    constexpr Vec3() {
    }

    constexpr Vec3(const T v)
        : x{ v },
          y{ v },
          z{ v } {
    }

    constexpr Vec3(const T x, const T y, const T z)
        : x{ x },
          y{ y },
          z{ z } {
    }

    T&
    operator[](const u64 idx) {
        assert(idx < 3);
        return e[idx];
    }

    const T&
    operator[](const u64 idx) const {
        assert(idx < 3);
        return e[idx];
    }

    constexpr T
    length() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    constexpr T
    unit() const {
        const T len = length();
        return {
            .x = x / len,
            .y = y / len,
            .z = z / len,
        };
    }

    constexpr T
    dot(const Vec3& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    constexpr Vec3
    cross(const Vec3& other) const {
        return {
            .x = (y * other.z) - (z * other.y),
            .y = (z * other.x) - (x * other.z),
            .z = (x * other.y) - (y * other.x),
        };
    }
};

template <typename T>
union Vec4 {
    struct {
        T x = 0;
        T y = 0;
        T z = 0;
        T w = 0;
    };

    T e[4];

    constexpr Vec4() {
    }

    constexpr Vec4(const T v)
        : x{ v },
          y{ v },
          z{ v },
          w{ v } {
    }

    constexpr Vec4(const T x, const T y, const T z, const T w)
        : x{ x },
          y{ y },
          z{ z },
          w{ w } {
    }

    T&
    operator[](const u64 idx) {
        assert(idx < 4);
        return e[idx];
    }

    const T&
    operator[](const u64 idx) const {
        assert(idx < 4);
        return e[idx];
    }

    constexpr T
    length() const {
        return sqrt((x * x) + (y * y) + (z * z) + (w * w));
    }

    constexpr T
    unit() const {
        const T len = length();
        return {
            .x = x / len,
            .y = y / len,
            .z = z / len,
            .w = w / len,
        };
    }

    constexpr T
    dot(const Vec4& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    }
};

template <typename T>
constexpr Vec2<T>
operator+(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator+(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator+(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator-(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator-(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator-(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator-(const Vec2<T>& a) {
    return {
        .x = -a.x,
        .y = -a.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator-(const Vec3<T>& a) {
    return {
        .x = -a.x,
        .y = -a.y,
        .z = -a.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator-(const Vec4<T>& a) {
    return {
        .x = -a.x,
        .y = -a.y,
        .z = -a.z,
        .w = -a.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator*(const Vec2<T>& a, const T b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
    };
}

template <typename T>
constexpr Vec3<T>
operator*(const Vec3<T>& a, const T b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b,
    };
}

template <typename T>
constexpr Vec4<T>
operator*(const Vec4<T>& a, const T b) {
    return {
        .x = a.x * b,
        .y = a.y * b,
        .z = a.z * b,
        .w = a.w * b,
    };
}

template <typename T>
constexpr Vec2<T>
operator/(const Vec2<T>& a, const T b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
    };
}

template <typename T>
constexpr Vec3<T>
operator/(const Vec3<T>& a, const T b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
        .z = a.z / b,
    };
}

template <typename T>
constexpr Vec4<T>
operator/(const Vec4<T>& a, const T b) {
    return {
        .x = a.x / b,
        .y = a.y / b,
        .z = a.z / b,
        .w = a.w / b,
    };
}

template <typename T>
struct Mat2 {
    Vec2<T> e[2] = {};

    constexpr Mat2() {
    }

    constexpr Mat2(const T v)
        : e{ Vec2<T>(v, 0), Vec2<T>(0, v) } {
    }

    constexpr Mat2(const T x1, const T y1, const T x2, const T y2)
        : e{ Vec2<T>(x1, y1), Vec2<T>(x2, y2) } {
    }

    constexpr Mat2(const Vec2<T>& x, const Vec2<T>& y)
        : e{ x, y } {
    }

    Vec2<T>&
    operator[](const u64 idx) {
        assert(idx < 2);
        return e[idx];
    }

    const Vec2<T>&
    operator[](const u64 idx) const {
        assert(idx < 2);
        return e[idx];
    }
};

template <typename T>
struct Mat3 {
    Vec3<T> e[3] = {};

    constexpr Mat3() {
    }

    constexpr Mat3(const T v)
        : e{ Vec3<T>(v, 0, 0), Vec3<T>(0, v, 0), Vec3<T>(0, 0, v) } {
    }

    constexpr Mat3(
        const T x1,
        const T y1,
        const T z1,
        const T x2,
        const T y2,
        const T z2,
        const T x3,
        const T y3,
        const T z3
    )
        : e{ Vec3<T>(x1, y1, z1), Vec3<T>(x2, y2, z2), Vec3<T>(x3, y3, z3) } {
    }

    constexpr Mat3(const Vec3<T>& x, const Vec3<T>& y, const Vec3<T>& z)
        : e{ x, y, z } {
    }

    Vec3<T>&
    operator[](const u64 idx) {
        assert(idx < 3);
        return e[idx];
    }

    const Vec2<T>&
    operator[](const u64 idx) const {
        assert(idx < 3);
        return e[idx];
    }
};

template <typename T>
struct Mat4 {
    Vec4<T> e[4] = {};

    constexpr Mat4() {
    }

    constexpr Mat4(const T v)
        : e{ Vec4<T>(v, 0, 0, 0), Vec4<T>(0, v, 0, 0), Vec4<T>(0, 0, v, 0), Vec4<T>(0, 0, 0, v) } {
    }

    constexpr Mat4(
        const T x1,
        const T y1,
        const T z1,
        const T w1,
        const T x2,
        const T y2,
        const T z2,
        const T w2,
        const T x3,
        const T y3,
        const T z3,
        const T w3,
        const T x4,
        const T y4,
        const T z4,
        const T w4
    )
        : e{ Vec4<T>(x1, y1, z1, w1),
             Vec4<T>(x2, y2, z2, w2),
             Vec4<T>(x3, y3, z3, w3),
             Vec4<T>(x4, y4, z4, w4) } {
    }

    constexpr Mat4(const Vec4<T>& x, const Vec4<T>& y, const Vec4<T>& z)
        : e{ x, y, z } {
    }

    Vec4<T>&
    operator[](const u64 idx) {
        assert(idx < 4);
        return e[idx];
    }

    const Vec2<T>&
    operator[](const u64 idx) const {
        assert(idx < 4);
        return e[idx];
    }
};

using Mat2f = Mat2<f32>;
using Mat3f = Mat3<f32>;
using Mat4f = Mat4<f32>;

template <typename T>
constexpr Mat2<T>
operator+(const Mat2<T>& a, const Mat2<T>& b) {
    Mat2<T> out;
    for (u32 j = 0; j < 2; ++j) {
        for (u32 i = 0; i < 2; ++i) {
            out[j][i] = a[j][i] + b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat2<T>
operator-(const Mat2<T>& a, const Mat2<T>& b) {
    Mat2<T> out;
    for (u32 j = 0; j < 2; ++j) {
        for (u32 i = 0; i < 2; ++i) {
            out[j][i] = a[j][i] - b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat2<T>
operator*(const Mat2<T>& a, const T b) {
    Mat2<T> out;
    for (u32 j = 0; j < 2; ++j) {
        for (u32 i = 0; i < 2; ++i) {
            out[j][i] = a[j][i] * b;
        }
    }
    return out;
}

template <typename T>
constexpr Mat3<T>
operator+(const Mat3<T>& a, const Mat3<T>& b) {
    Mat3<T> out;
    for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 3; ++i) {
            out[j][i] = a[j][i] + b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat3<T>
operator-(const Mat3<T>& a, const Mat3<T>& b) {
    Mat3<T> out;
    for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 3; ++i) {
            out[j][i] = a[j][i] - b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat3<T>
operator*(const Mat3<T>& a, const T b) {
    Mat3<T> out;
    for (u32 j = 0; j < 3; ++j) {
        for (u32 i = 0; i < 3; ++i) {
            out[j][i] = a[j][i] * b;
        }
    }
    return out;
}

template <typename T>
constexpr Mat4<T>
operator+(const Mat4<T>& a, const Mat4<T>& b) {
    Mat4<T> out;
    for (u32 j = 0; j < 4; ++j) {
        for (u32 i = 0; i < 4; ++i) {
            out[j][i] = a[j][i] + b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat4<T>
operator-(const Mat4<T>& a, const Mat4<T>& b) {
    Mat4<T> out;
    for (u32 j = 0; j < 4; ++j) {
        for (u32 i = 0; i < 4; ++i) {
            out[j][i] = a[j][i] - b[j][i];
        }
    }
    return out;
}

template <typename T>
constexpr Mat4<T>
operator*(const Mat4<T>& a, const T b) {
    Mat4<T> out;
    for (u32 j = 0; j < 4; ++j) {
        for (u32 i = 0; i < 4; ++i) {
            out[j][i] = a[j][i] * b;
        }
    }
    return out;
}
