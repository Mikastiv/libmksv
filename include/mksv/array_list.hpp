#pragma once

#include "mem.hpp"

namespace mksv {

template <typename T>
struct ArrayList {
    mem::Allocator alloc;
    mem::Span<T> items;
    u64 cap;

    static ArrayList
    init(const mem::Allocator alloc) {
        return {
            .alloc = alloc,
            .items = {.ptr = nullptr, .len = 0},
            .cap = 0,
        };
    }

    bool
    append(const T item);

    bool
    append(const mem::Span<T> range);

    void
    deinit() {
        alloc.free(items);
    }
};

} // namespace mksv
