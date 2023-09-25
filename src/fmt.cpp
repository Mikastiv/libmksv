#include "fmt.hpp"

#include "utils.hpp"

namespace mksv {
namespace fmt {

static u64 pow10_table[] = {
    1,
    10,
    100,
    1'000,
    10'000,
    100'000,
    1'000'000,
    10'000'000,
    100'000'000,
    1'000'000'000,
    10'000'000'000,
    100'000'000'000,
    1'000'000'000'000,
    10'000'000'000'000,
    100'000'000'000'000,
    1'000'000'000'000'000,
};

static constexpr u64
parse_digits(const Str str, u64* idx, const u64 base) {
    u64 number = 0;
    while (*idx < str.len && is_digit(str.ptr[*idx])) {
        number *= base;
        number += str.ptr[*idx] - '0';
        ++(*idx);
    }
    return number;
}

template <typename T>
constexpr bool
convert_float(const Str str, T* out) {
    if (str.len == 0) return false;

    u64 idx = 0;
    const bool negative = str.ptr[idx] == '-';
    if (str.ptr[idx] == '-' || str.ptr[idx] == '+') ++idx;

    u64 n_digits = 0;
    const u64 integer_start = idx;
    const u64 integer = parse_digits(str, &idx, 10);
    n_digits += idx - integer_start;

    if (idx < str.len && str.ptr[idx] == '.') ++idx;
    const u64 decimal_start = idx;
    const u64 decimal = parse_digits(str, &idx, 10);
    const u64 exponent = idx - decimal_start;
    n_digits += exponent;

    if (n_digits > PARSE_FLOAT_MAX_DIGITS) return false;

    const u64 whole_number = (integer * pow10_table[exponent]) + decimal;
    T float_number = (T)whole_number;
    float_number /= pow10_table[exponent];
    if (negative) float_number = -float_number;

    if (idx != str.len) return false;

    *out = float_number;

    return true;
}

bool
parse_float(const Str str, f32* out) {
    return convert_float(str, out);
}

bool
parse_float(const Str str, f64* out) {
    return convert_float(str, out);
}

template <typename T>
constexpr bool
convert_uint(const Str str, const T base, T* out) {
    if (str.len == 0) return false;

    u64 idx = 0;
    if (str.ptr[idx] == '-') return false;

    if (str.ptr[idx] == '+') ++idx;

    const u64 number = parse_digits(str, &idx, base);
    if (idx != str.len) return false;
    *out = (T)number;

    return true;
}

template <typename T>
constexpr bool
convert_int(const Str str, const T base, T* out) {
    if (str.len == 0) return false;

    u64 idx = 0;
    const T sign = str.ptr[idx] == '-' ? -1 : 1;
    if (str.ptr[idx] == '-' || str.ptr[idx] == '+') ++idx;

    T number = 0;
    while (is_digit(str.ptr[idx])) {
        number *= base;
        number += (str.ptr[idx] - '0') * sign;
        ++idx;
    }

    if (idx != str.len) return false;
    *out = number;

    return true;
}

bool
parse_int(const Str str, const u8 base, u8* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const u16 base, u16* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const u32 base, u32* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const u64 base, u64* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const i8 base, i8* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i16 base, i16* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i32 base, i32* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i64 base, i64* out) {
    return convert_int(str, base, out);
}

} // namespace fmt
} // namespace mksv
