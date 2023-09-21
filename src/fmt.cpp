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

static u64
parse_digits(const Str str, u64* idx) {
    u64 number = 0;
    while (*idx < str.len && is_digit(str.ptr[*idx])) {
        number *= 10;
        number += str.ptr[*idx] - '0';
        ++(*idx);
    }
    return number;
}

template <typename T>
bool
convert_float(const Str str, T* out) {
    if (str.len == 0) return false;

    u64 idx = 0;
    const bool negative = str.ptr[idx] == '-';
    if (str.ptr[idx] == '-' || str.ptr[idx] == '+') ++idx;

    u64 n_digits = 0;
    const u64 integer_start = idx;
    const u64 integer = parse_digits(str, &idx);
    n_digits += idx - integer_start;

    if (idx < str.len && str.ptr[idx] == '.') ++idx;
    const u64 decimal_start = idx;
    const u64 decimal = parse_digits(str, &idx);
    const u64 exponent = idx - decimal_start;
    n_digits += exponent;

    if (n_digits > PARSE_FLOAT_MAX_DIGITS) return false;

    const u64 whole_number = (integer * pow10_table[exponent]) + decimal;
    T float_number = (T)whole_number;
    float_number /= pow10_table[exponent];
    if (negative) float_number = -float_number;

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

} // namespace fmt
} // namespace mksv
