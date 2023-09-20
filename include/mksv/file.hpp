#pragma once

#include "mem.hpp"

namespace mksv {
namespace io {

[[nodiscard]] bool
read_file(const Str filename, Str* out_str);

}
} // namespace mksv
