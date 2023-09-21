#pragma once

#include "mem.hpp"

namespace mksv {
namespace fmt {

[[nodiscard]]
bool parse_float(const Str str, f32* out);

[[nodiscard]]
bool parse_float(const Str str, f64* out);

}

} // namespace mksv
