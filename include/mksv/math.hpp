#pragma once

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

} // namespace math
} // namespace mksv
