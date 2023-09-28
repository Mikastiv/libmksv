#pragma once

#include "assert.hpp"
#include "io.hpp"
#include "mem.hpp"


namespace mksv {
namespace fmt {

constexpr u64 PARSE_FLOAT_MAX_DIGITS = 10;

// Fast float parsing for <= PARSE_FLOAT_MAX_DIGITS digits. Only accepts base 10
[[nodiscard]] bool
parse_float(const Str str, f32* out);

// Fast float parsing for <= PARSE_FLOAT_MAX_DIGITS digits. Only accepts base 10
[[nodiscard]] bool
parse_float(const Str str, f64* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u8 base, u8* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u8 base, u16* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u8 base, u32* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u8 base, u64* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i8 base, i8* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i8 base, i16* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i8 base, i32* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i8 base, i64* out);

enum struct FormatSpecifier {
    Unsigned8,
    Unsigned16,
    Unsigned32,
    Unsigned64,
    Signed8,
    Signed16,
    Signed32,
    Signed64,
    String,
    Pointer,
    Unknown,
};

FormatSpecifier
_get_format_specifier(const Str fmt_string);

enum struct FormatBase {
    Decimal,
    HexadecimalLower,
    HexadecimalUpper,
    Octal,
    Binary,
    Unknown,
};

FormatBase
_get_base(const Str base_str);

Str
_format_string(const Str buffer, const Str str);

Str
_format_integer(const Str buffer, const u8 num, const FormatBase base);

Str
_format_integer(const Str buffer, const u16 num, const FormatBase base);

Str
_format_integer(const Str buffer, const u32 num, const FormatBase base);

Str
_format_integer(const Str buffer, const u64 num, const FormatBase base);

Str
_format_integer(const Str buffer, const i8 num, const FormatBase base);

Str
_format_integer(const Str buffer, const i16 num, const FormatBase base);

Str
_format_integer(const Str buffer, const i32 num, const FormatBase base);

Str
_format_integer(const Str buffer, const i64 num, const FormatBase base);

Str
_format_pointer(const Str buffer, const u64 address, const FormatBase base);

template <typename T>
Str
_format_dispatch(
    const Str buffer,
    const FormatSpecifier spec,
    const T* value,
    const FormatBase base
) {
    switch (spec) {
        case FormatSpecifier::String: {
            return _format_string(buffer, *(const Str*)value);
        } break;
        case FormatSpecifier::Unsigned8: {
            return _format_integer(buffer, *(const u8*)value, base);
        } break;
        case FormatSpecifier::Unsigned16: {
            return _format_integer(buffer, *(const u16*)value, base);
        } break;
        case FormatSpecifier::Unsigned32: {
            return _format_integer(buffer, *(const u32*)value, base);
        } break;
        case FormatSpecifier::Unsigned64: {
            return _format_integer(buffer, *(const u64*)value, base);
        } break;
        case FormatSpecifier::Signed8: {
            return _format_integer(buffer, *(const i8*)value, base);
        } break;
        case FormatSpecifier::Signed16: {
            return _format_integer(buffer, *(const i16*)value, base);
        } break;
        case FormatSpecifier::Signed32: {
            return _format_integer(buffer, *(const i32*)value, base);
        } break;
        case FormatSpecifier::Signed64: {
            return _format_integer(buffer, *(const i64*)value, base);
        } break;
        case FormatSpecifier::Pointer: {
            return _format_pointer(buffer, *(const u64*)value, base);
        } break;
        default: {
            return Str::null();
        } break;
    }
}

u64
_count_specifiers(const Str fmt);

Str
_format_inner(const Str buffer, const Str fmt);

template <typename T, typename... Args>
Str
_format_inner(const Str buffer, const Str fmt, T value, Args... args) {
    u64 write_idx = 0;

    u64 idx = 0;
    while (idx < fmt.len) {
        const u64 start = idx;

        while (idx < fmt.len && fmt.ptr[idx] != '{' && fmt.ptr[idx] != '}') ++idx;

        bool restart_loop = false;
        u64 end = idx;
        if (idx + 1 < fmt.len && fmt.ptr[idx + 1] == fmt.ptr[idx]) {
            end += 1;
            idx += 2;
            restart_loop = true;
        }

        if (start != end) {
            const auto litteral = Str{ &fmt.ptr[start], end - start };
            if (litteral.len > buffer.len - write_idx) return Str::null();
            mem::copy(Str{ buffer.ptr + write_idx, litteral.len }, litteral);
            write_idx += litteral.len;
        }

        if (restart_loop) continue;

        if (idx >= fmt.len) break;

        assert(fmt.ptr[idx] == '{');
        if (fmt.ptr[idx] == '}') return Str::null();

        ++idx;

        const u64 fmt_start = idx;
        while (idx < fmt.len && fmt.ptr[idx] != '}') ++idx;
        const u64 fmt_end = idx;

        if (idx >= fmt.len) return Str::null();

        assert(fmt.ptr[idx] == '}');
        ++idx;

        const auto fmt_string = Str{ fmt.ptr + fmt_start, fmt_end - fmt_start };
        u64 fmt_specifier_start = fmt_start;
        u64 fmt_specifier_end = fmt_end;

        bool has_base = false;
        u64 colon = 0;
        if (mem::find(fmt_string, (Str) ":", &colon)) {
            has_base = true;
            fmt_specifier_end = fmt_start + colon;
        }

        FormatSpecifier spec = _get_format_specifier(Str{
            fmt.ptr + fmt_specifier_start,
            fmt_specifier_end - fmt_specifier_start,
        });
        if (spec == FormatSpecifier::Unknown) return Str::null();

        FormatBase base =
            spec == FormatSpecifier::Pointer ? FormatBase::HexadecimalUpper : FormatBase::Decimal;
        if (has_base) {
            const u64 base_start = fmt_specifier_end + 1;
            const u64 base_end = fmt_end;
            if (base_start != base_end) {
                base = _get_base(Str{ fmt.ptr + base_start, base_end - base_start });
            }
        }
        if (base == FormatBase::Unknown) return Str::null();

        const auto remaining_buffer = Str{ buffer.ptr + write_idx, buffer.len - write_idx };
        const auto format_result = _format_dispatch(remaining_buffer, spec, &value, base);
        if (format_result.ptr == nullptr) return Str::null();
        write_idx += format_result.len;

        const Str result_str = _format_inner(
            Str{ buffer.ptr + write_idx, buffer.len - write_idx },
            Str{ fmt.ptr + idx, fmt.len - idx },
            args...
        );
        if (result_str.ptr == nullptr) return Str::null();

        return Str{ buffer.ptr, write_idx + result_str.len };
    }

    return Str{ buffer.ptr, write_idx };
}

inline Str
format(const Str buffer, const Str fmt) {
    return _format_inner(buffer, fmt);
}

// Format string
// Syntax: {[type]:[base]}
//
// type: s for string; u8, u16, i8, i16, etc, for integers; p for address
//
// base: d for decimal, o for octal, b for binary, x or X for hexadecimal
template <typename T, typename... Args>
Str
format(const Str buffer, const Str fmt, T value, Args... args) {
    assert(sizeof...(args) + 1 == _count_specifiers(fmt));

    return _format_inner(buffer, fmt, value, args...);
}

template <typename T, typename... Args>
void
print_stdout(const Str fmt, T value, Args... args) {
    u8 buffer[2048] = {};
    const Str msg = format(buffer, fmt, value, args...);
    io::write_stdout(msg);
}

template <typename T, typename... Args>
void
print_stderr(const Str fmt, T value, Args... args) {
    u8 buffer[2048] = {};
    const Str msg = format(str(buffer), fmt, value, args...);
    io::write_stderr(msg);
}

} // namespace fmt

} // namespace mksv
