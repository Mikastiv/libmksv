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
parse_digits(const Str str, u64* idx, const u8 base) {
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
convert_uint(const Str str, const u8 base, T* out) {
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
convert_int(const Str str, const i8 base, T* out) {
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
parse_int(const Str str, const u8 base, u16* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const u8 base, u32* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const u8 base, u64* out) {
    return convert_uint(str, base, out);
}

bool
parse_int(const Str str, const i8 base, i8* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i8 base, i16* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i8 base, i32* out) {
    return convert_int(str, base, out);
}

bool
parse_int(const Str str, const i8 base, i64* out) {
    return convert_int(str, base, out);
}

FormatSpecifier
_get_format_specifier(const Str fmt_string) {
    if (mem::equal(fmt_string, (Str) "u8")) return FormatSpecifier::Unsigned8;
    if (mem::equal(fmt_string, (Str) "u16")) return FormatSpecifier::Unsigned16;
    if (mem::equal(fmt_string, (Str) "u32")) return FormatSpecifier::Unsigned32;
    if (mem::equal(fmt_string, (Str) "u64")) return FormatSpecifier::Unsigned64;
    if (mem::equal(fmt_string, (Str) "i8")) return FormatSpecifier::Signed8;
    if (mem::equal(fmt_string, (Str) "i16")) return FormatSpecifier::Signed16;
    if (mem::equal(fmt_string, (Str) "i32")) return FormatSpecifier::Signed32;
    if (mem::equal(fmt_string, (Str) "i64")) return FormatSpecifier::Signed64;
    if (mem::equal(fmt_string, (Str) "s")) return FormatSpecifier::String;
    return FormatSpecifier::Unknown;
}

FormatBase
_get_base(const Str base_str) {
    if (mem::equal(base_str, (Str) "b")) return FormatBase::Binary;
    if (mem::equal(base_str, (Str) "o")) return FormatBase::Octal;
    if (mem::equal(base_str, (Str) "d")) return FormatBase::Decimal;
    if (mem::equal(base_str, (Str) "x")) return FormatBase::HexadecimalLower;
    if (mem::equal(base_str, (Str) "X")) return FormatBase::HexadecimalUpper;
    return FormatBase::Unknown;
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
    mem::copy(Str{ buffer.ptr, str.len }, str);
    return Str{ buffer.ptr, str.len };
}

static const Str
get_base_characters(const FormatBase base) {
    switch (base) {
        case FormatBase::Decimal:
            return "0123456789";
        case FormatBase::HexadecimalLower:
            return "0123456789abcdef";
        case FormatBase::HexadecimalUpper:
            return "0123456789ABCDEF";
        case FormatBase::Octal:
            return "01234567";
        case FormatBase::Binary:
            return "01";
        case FormatBase::Unknown:
            return "U";
    }
}

template <typename T>
static i32
numlen(T num, const u64 base) {
    i32 len = 0;

    while (num / (T)base != 0) {
        num /= (T)base;
        ++len;
    }

    return len + 1;
}

template <typename T>
static Str
itoa(const Str buffer, T num, const Str base) {
    i32 len = numlen(num, base.len);
    u64 ret_len = (u64)len;
    i32 offset = 0;

    if (num < 0) ++ret_len;
    if (ret_len > buffer.len) return Str::null();

    if (num < 0) {
        offset = 1;
        buffer.ptr[0] = '-';
    }

    while (len > 0) {
        i32 n = (i32)num % (i32)base.len;
        if (n < 0) n = -n;
        --len;
        buffer.ptr[len + offset] = base.ptr[n];
        num /= (T)base.len;
    }

    return Str{ buffer.ptr, ret_len };
}

Str
_format_integer(const Str buffer, const u8 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const u16 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const u32 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const u64 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const i8 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const i16 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const i32 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
}

Str
_format_integer(const Str buffer, const i64 num, const FormatBase base) {
    return itoa(buffer, num, get_base_characters(base));
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
            const auto litteral = Str{ &fmt.ptr[start], end - start };
            if (litteral.len > buffer.len - write_idx) return Str::null();
            mem::copy(Str{ buffer.ptr + write_idx, litteral.len }, litteral);
            write_idx += litteral.len;
        }
    }

    return Str{ buffer.ptr, write_idx };
}

} // namespace fmt
} // namespace mksv
