#pragma once

#include "mem.hpp"

namespace mksv {
namespace io {

// Returned Str is null terminated
[[nodiscard]] bool
read_file(const mem::Allocator allocator, const Str filename, mem::Slice<u8>* out_str);

[[nodiscard]] bool
write_file(const Str filename, const Str buffer);

bool
write_stdout(const Str buffer);

bool
write_stderr(const Str buffer);

} // namespace io
} // namespace mksv
