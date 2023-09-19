#pragma once

#include "mem.hpp"
#include "singly_linked_list.hpp"

namespace mksv {
namespace heap {

mem::Allocator
system_allocator();

struct ArenaAllocator {
    typedef SinglyLinkedList<mem::Slice<u8>> Stack;
    typedef Stack::Node Node;
    static constexpr u64 NODE_SIZE = sizeof(Node);

    mem::Allocator inner_allocator;
    Stack stack;
    u64 end_idx;

    static ArenaAllocator
    init(const mem::Allocator inner);

    mem::Allocator
    allocator() const;

    void
    deinit();
};

} // namespace heap
} // namespace mksv
