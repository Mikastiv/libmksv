#pragma once

#include "mem.hpp"

namespace mksv {

template <typename T>
struct ArrayList {
    mem::Allocator allocator;
    mem::Span<T> items;
    u64 cap;

    ArrayList() = delete;

    static ArrayList
    init(const mem::Allocator allocator) {
        return {
            .allocator = allocator,
            .items = {.ptr = nullptr, .len = 0},
            .cap = 0,
        };
    }

    bool
    append(const T& item) {
        append({ .ptr = &item, .len = 1 });
    }

    bool
    append(const mem::Span<T> range);

    void
    deinit() {
        allocator.free(items);
        items.ptr = nullptr;
        items.len = 0;
        cap = 0;
    }
};

} // namespace mksv
