#pragma once

#include "mem.hpp"

namespace mksv {
namespace io {

[[nodiscard]] bool
read_file(mem::Allocator allocator, const Str filename, Str* out_str);

[[nodiscard]] bool
write_file(const Str filename, const Str buffer);

} // namespace io
} // namespace mksv
