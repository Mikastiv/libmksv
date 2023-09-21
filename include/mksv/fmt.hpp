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

} // namespace fmt

} // namespace mksv
