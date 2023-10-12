#pragma once

#include "mem.hpp"

namespace mksv {
namespace hash {

u64
hash(const mem::Slice<u8> s);

u64
hash_combine(const u64 seed, const u8 byte);

} // namespace hash
} // namespace mksv
