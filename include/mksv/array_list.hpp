#pragma once

#include "math.hpp"
#include "mem.hpp"

namespace mksv {

template <typename T>
struct ArrayList {
    mem::Allocator allocator;
    mem::Span<T> items;
    u64 size;

    static ArrayList
    init(const mem::Allocator allocator) {
        return {
            .allocator = allocator,
            .items = {.ptr = nullptr, .len = 0},
            .size = 0,
        };
    }

    bool
    append(const T& item) {
        append({ .ptr = &item, .len = 1 });
    }

    bool
    append(const mem::Span<T> range) {
        if (!ensure_capacity(range.len)) return false;

        mem::copy({.ptr = items.ptr + size, .len = range.len}, range);
        size += range.len;

        return true;
    }

    void
    deinit() {
        allocator.free(items);
        items.ptr = nullptr;
        items.len = 0;
        size = 0;
    }

    constexpr u64
    max_size() const {
        return math::MAX_U64 / sizeof(T);
    }

private:
    constexpr u64
    calculate_new_size(const u64 added_size) const {
        if (items.len > max_size() / 2) return max_size();
        if (added_size > max_size() / 2) return max_size();

        u64 new_cap = items.len * 2;
        if (new_cap - size < added_size) {
            new_cap = items.len + added_size;
        }

        return new_cap;
    }

    bool
    ensure_capacity(const u64 added_size) {
        if (items.len - size >= added_size) return true;

        u64 new_cap = calculate_new_size(size, added_size);
        if (new_cap == items.len) return false;

        if (allocator.resize(items, new_cap)) return true;

        const mem::Span<T> new_items = allocator.alloc<T>(new_cap);
        if (new_items.ptr == nullptr) return false;

        mem::copy(
            { .ptr = new_items.ptr, .len = size },
            { .ptr = items.ptr, .len = size }
        );

        allocator.free(items);
        items = new_items;

        return true;
    }
};

} // namespace mksv
