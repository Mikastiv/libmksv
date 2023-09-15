#pragma once

#include "mem.hpp"

namespace mksv {

struct ArenaAllocator {
    mem::Allocator system_allocator;
};

} // namespace mksv
