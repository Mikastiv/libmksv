#pragma once

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

} // namespace fmt

} // namespace mksv
