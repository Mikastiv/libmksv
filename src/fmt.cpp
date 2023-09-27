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

FormatSpecifer
_get_format_specifier(const Str fmt_string) {
    if (mem::equal(fmt_string, str("d"))) return FormatSpecifer::Decimal;
    if (mem::equal(fmt_string, str("x"))) return FormatSpecifer::HexadecimalLower;
    if (mem::equal(fmt_string, str("X"))) return FormatSpecifer::HexadecimalUpper;
    if (mem::equal(fmt_string, str("b"))) return FormatSpecifer::Binary;
    if (mem::equal(fmt_string, str("s"))) return FormatSpecifer::String;
    return FormatSpecifer::Unknown;
}

u64
_count_specifiers(const Str fmt) {
    u64 count = 0;
    u64 idx = 0;
    while (idx < fmt.len) {
        while (idx < fmt.len && fmt.ptr[idx] != '{' && fmt.ptr[idx] != '}') ++idx;

        bool restart_loop = false;
        if (idx + 1 < fmt.len && fmt.ptr[idx + 1] == fmt.ptr[idx]) {
            idx += 2;
            restart_loop = true;
        }

        if (restart_loop) continue;

        if (idx >= fmt.len) break;

        assert(fmt.ptr[idx] == '{');
        assert(fmt.ptr[idx] != '}');

        ++idx;

        while (idx < fmt.len && fmt.ptr[idx] != '}') ++idx;

        assert(idx < fmt.len);
        assert(fmt.ptr[idx] == '}');

        ++idx;
        ++count;
    }

    return count;
}

Str
_format_string(const Str buffer, const Str str) {
    mem::copy({ .ptr = buffer.ptr, .len = str.len }, str);
    return { .ptr = buffer.ptr, .len = str.len };
}

Str
_format_inner(const Str buffer, const Str fmt) {
    u64 write_idx = 0;

    u64 idx = 0;
    while (idx < fmt.len) {
        const u64 start = idx;

        while (idx < fmt.len && fmt.ptr[idx] != '{' && fmt.ptr[idx] != '}') ++idx;

        u64 end = idx;
        if (idx + 1 < fmt.len && fmt.ptr[idx + 1] == fmt.ptr[idx]) {
            end += 1;
            idx += 2;
        }

        if (start != end) {
            const Str litteral = { .ptr = &fmt.ptr[start], .len = end - start };
            if (litteral.len > buffer.len - write_idx) return Str::null();
            mem::copy({ .ptr = buffer.ptr + write_idx, .len = litteral.len }, litteral);
            write_idx += litteral.len;
        }
    }

    return { .ptr = buffer.ptr, .len = write_idx };
}

} // namespace fmt
} // namespace mksv
