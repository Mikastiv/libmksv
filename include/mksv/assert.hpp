#pragma once

#include "types.hpp"

namespace mksv {

void
assert_message(const char* expr, const char* file, const u64 line_number);

} // namespace mksv

#define assert(expr) (void)((!!(expr)) || (mksv::assert_message((#expr), __FILE__, __LINE__), 0))
