#pragma once

#include "types.hpp"
#include "math.hpp"

namespace mksv {
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

    constexpr Vec2
    unit() const {
        const T len = length();
        return {
            x / len,
            y / len,
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
        return math::sqrt((x * x) + (y * y) + (z * z));
    }

    constexpr Vec3
    unit() const {
        const T len = length();
        return {
            x / len,
            y / len,
            z / len,
        };
    }

    constexpr T
    dot(const Vec3& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    constexpr Vec3
    cross(const Vec3& other) const {
        return {
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x),
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

    constexpr Vec4
    unit() const {
        const T len = length();
        return {
            x / len,
            y / len,
            z / len,
            w / len,
        };
    }

    constexpr T
    dot(const Vec4& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    }
};

using Vec2f = Vec2<f32>;
using Vec3f = Vec3<f32>;
using Vec4f = Vec4<f32>;

template <typename T>
constexpr Vec2<T>
operator+(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        a.x + b.x,
        a.y + b.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator+(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator+(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator-(const Vec2<T>& a, const Vec2<T>& b) {
    return {
        a.x - b.x,
        a.y - b.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator-(const Vec3<T>& a, const Vec3<T>& b) {
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator-(const Vec4<T>& a, const Vec4<T>& b) {
    return {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator-(const Vec2<T>& a) {
    return {
        -a.x,
        -a.y,
    };
}

template <typename T>
constexpr Vec3<T>
operator-(const Vec3<T>& a) {
    return {
        -a.x,
        -a.y,
        -a.z,
    };
}

template <typename T>
constexpr Vec4<T>
operator-(const Vec4<T>& a) {
    return {
        -a.x,
        -a.y,
        -a.z,
        -a.w,
    };
}

template <typename T>
constexpr Vec2<T>
operator*(const Vec2<T>& a, const T b) {
    return {
        a.x * b,
        a.y * b,
    };
}

template <typename T>
constexpr Vec2<T>
operator*(const T a, const Vec2<T>& b) {
    return b * a;
}

template <typename T>
constexpr Vec3<T>
operator*(const Vec3<T>& a, const T b) {
    return {
        a.x * b,
        a.y * b,
        a.z * b,
    };
}

template <typename T>
constexpr Vec3<T>
operator*(const T a, const Vec3<T>& b) {
    return b * a;
}

template <typename T>
constexpr Vec4<T>
operator*(const Vec4<T>& a, const T b) {
    return {
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b,
    };
}

template <typename T>
constexpr Vec4<T>
operator*(const T b, const Vec4<T>& a) {
    return b * a;
}

template <typename T>
constexpr Vec2<T>
operator/(const Vec2<T>& a, const T b) {
    return {
        a.x / b,
        a.y / b,
    };
}

template <typename T>
constexpr Vec3<T>
operator/(const Vec3<T>& a, const T b) {
    return {
        a.x / b,
        a.y / b,
        a.z / b,
    };
}

template <typename T>
constexpr Vec4<T>
operator/(const Vec4<T>& a, const T b) {
    return {
        a.x / b,
        a.y / b,
        a.z / b,
        a.w / b,
    };
}
} // namespace mksv
