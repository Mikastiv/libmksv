#pragma once

#include "mem.hpp"

namespace mksv {
namespace hash {

u64
hash(const mem::Slice<u8> s);

} // namespace hash
} // namespace mksv
