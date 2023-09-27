#pragma once

#include "assert.hpp"
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
parse_int(const Str str, const u16 base, u16* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u32 base, u32* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const u64 base, u64* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i8 base, i8* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i16 base, i16* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i32 base, i32* out);

// Integer parsing without overflow checks
[[nodiscard]] bool
parse_int(const Str str, const i64 base, i64* out);

enum struct FormatSpecifer {
    Decimal,
    HexadecimalLower,
    HexadecimalUpper,
    Binary,
    String,
    Unknown,
};

FormatSpecifer
_get_format_specifier(const Str str);

Str
_format_string(const Str buffer, const Str str);

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
            const Str litteral = { .ptr = &fmt.ptr[start], .len = end - start };
            if (litteral.len > buffer.len - write_idx) return Str::null();
            mem::copy({ .ptr = buffer.ptr + write_idx, .len = litteral.len }, litteral);
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

        const Str fmt_string = { .ptr = fmt.ptr + fmt_start, .len = fmt_end - fmt_start };
        FormatSpecifer spec = _get_format_specifier(fmt_string);
        if (spec == FormatSpecifer::Unknown) return Str::null();

        Str format_result = Str::null();
        const Str remaining_buffer = {
            .ptr = buffer.ptr + write_idx,
            .len = buffer.len - write_idx,
        };
        switch (spec) {
            case FormatSpecifer::String: {
                format_result = _format_string(remaining_buffer, value);
            } break;
            default: {
                format_result = Str::null();
            } break;
        }
        if (format_result.ptr == nullptr) return Str::null();
        write_idx += format_result.len;

        const Str result_str = _format_inner(
            { .ptr = buffer.ptr + write_idx, .len = buffer.len - write_idx },
            { .ptr = fmt.ptr + idx, .len = fmt.len - idx },
            args...
        );
        if (result_str.ptr == nullptr) return Str::null();

        return {
            .ptr = buffer.ptr,
            .len = write_idx + result_str.len,
        };
    }

    return { .ptr = buffer.ptr, .len = write_idx };
}

inline Str
format(const Str buffer, const Str fmt) {
    return _format_inner(buffer, fmt);
}

template <typename T, typename... Args>
Str
format(const Str buffer, const Str fmt, T value, Args... args) {
    assert(sizeof...(args) + 1 == _count_specifiers(fmt));

    return _format_inner(buffer, fmt, value, args...);
}

} // namespace fmt

} // namespace mksv
