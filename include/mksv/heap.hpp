#pragma once

#include "mem.hpp"
#include "singly_linked_list.hpp"

namespace mksv {
namespace heap {

mem::Allocator
system_allocator();

struct MemoryBlock {
    mem::Span<u8> span;
};

struct ArenaAllocator {
    mem::Allocator inner_allocator;
    SinglyLinkedList<MemoryBlock> stack;

    typedef SinglyLinkedList<MemoryBlock>::Node Node;

    static ArenaAllocator
    init(const mem::Allocator inner);

    mem::Allocator
    allocator() const;

    void
    deinit();
};

} // namespace heap
} // namespace mksv
