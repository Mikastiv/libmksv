#pragma once

#include "math.hpp"
#include "mem.hpp"

namespace mksv {

template <typename T>
struct ArrayList {
    mem::Allocator allocator;
    mem::Slice<T> items;
    u64 size;

    [[nodiscard]] static ArrayList
    init(const mem::Allocator allocator) {
        return {
            .allocator = allocator,
            .items = {},
            .size = 0,
        };
    }

    [[nodiscard]] bool
    ensure_capacity(const u64 capacity) {
        if (items.len - size >= capacity) return true;

        u64 new_cap = calculate_new_size(capacity);
        if (new_cap == items.len) return false;

        if (allocator.resize(items, new_cap)) {
            items.len = new_cap;
            return true;
        }

        mem::Slice<T> new_items = {};
        if (!allocator.alloc<T>(new_cap, &new_items)) return false;

        mem::copy<T>(mem::Slice<T>{ new_items.ptr, size }, mem::Slice<T>{ items.ptr, size });

        allocator.free(items);
        items = new_items;

        return true;
    }

    [[nodiscard]] bool
    append(const mem::Slice<T> range) {
        if (!ensure_capacity(range.len)) return false;

        mem::copy(mem::Slice<T>{ items.ptr + size, range.len }, range);
        size += range.len;

        return true;
    }

    [[nodiscard]] bool
    append(T item) {
        const auto range = mem::Slice<T>{ &item, 1 };
        return append(range);
    }

    mem::Slice<T>
    slice() const {
        return mem::Slice<T>{ items.ptr, size };
    }

    T*
    begin() {
        return items.ptr;
    }

    const T*
    begin() const {
        return items.ptr;
    }

    T*
    end() {
        return items.ptr + size;
    }

    const T*
    end() const {
        return items.ptr + size;
    }

    T*
    last() {
        return items.ptr + size - 1;
    }

    void
    deinit() {
        allocator.free(items);
        items = {};
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
};

} // namespace mksv
