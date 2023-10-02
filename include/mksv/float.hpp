#pragma once

#include "types.hpp"

namespace mksv {
namespace flt {

constexpr u32 F32_SIGN_BIT = 0x8000'0000;
constexpr u32 F32_EXP_MASK = 0x7F80'0000;
constexpr u32 F32_MANTISSA_MASK = 0x007F'FFFF;
constexpr u32 F32_EXP_SHIFT = 23;
constexpr i32 F32_EXP_BIAS = 127;
constexpr f32 F32_EPSILON = 1.192092896e-07F;

} // namespace flt
} // namespace mksv
