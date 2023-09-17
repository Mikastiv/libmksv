#pragma once

#include "mem.hpp"

namespace mksv {
namespace heap {

mem::Allocator system_allocator();

struct ArenaAllocator {
    mem::Allocator inner_allocator;

    static ArenaAllocator init(const mem::Allocator inner);

    void deinit();
};

} // namespace heap
} // namespace mksv
