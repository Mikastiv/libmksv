#pragma once

#include "assert.hpp"
#include "types.hpp"

namespace mksv {
namespace mem {

template <typename T>
struct Slice {
    T* ptr;
    u64 len;

    constexpr Slice
    at(const u64 index) {
        assert(index < len);
        return {
            .ptr = ptr + index,
            .len = len - index,
        };
    }

    constexpr Slice
    sub(const u64 start, const u64 end) {
        assert(end >= start);
        assert(end <= len);
        return {
            .ptr = ptr + start,
            .len = end - start,
        };
    }
};

template <typename T>
constexpr bool
equal(const Slice<T> a, const Slice<T> b) {
    if (a.len != b.len) return false;
    if (a.ptr == b.ptr) return true;

    for (u64 i = 0; i < a.len; ++i) {
        if (a.ptr[i] != b.ptr[i]) return false;
    }

    return true;
}

template <typename T>
struct SplitIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;

    constexpr bool
    is_delimiter(const u64 index) {
        if (index + delimiter.len > buffer.len) return false;
        return equal(buffer.sub(index, index + delimiter.len), delimiter);
    }

    constexpr bool
    next(Slice<T>* value) {
        while (index < buffer.len && is_delimiter(index)) {
            index += delimiter.len;
        }

        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(end)) ++end;

        index += end - start;

        *value = buffer.sub(start, end);

        return true;
    }
};

template <typename T>
SplitIter<T>
split(const Slice<T> slice, const Slice<T> delimiter) {
    return {
        .buffer = slice,
        .delimiter = delimiter,
        .index = 0,
    };
}

using AllocFn = bool (*)(void*, const u64, const u64, Slice<u8>* out_block);
using ResizeFn =
    bool (*)(void* ctx, void* ptr, const u64 old_size, const u64 new_size, const u64 alignment);
using FreeFn = void (*)(void* ctx, void* ptr, const u64 size, const u64 alignment);

struct Allocator {
    void* ctx;

    struct VTable {
        AllocFn alloc_fn;
        ResizeFn resize_fn;
        FreeFn free_fn;
    } vtable;

    template <typename T>
    [[nodiscard]] bool
    alloc(const u64 len, Slice<T>* out_block) {
        Slice<u8> block = {};
        if (!vtable.alloc_fn(ctx, len * sizeof(T), alignof(T), &block)) return false;

        *out_block = {
            .ptr = (T*)block.ptr,
            .len = len,
        };

        return true;
    }

    template <typename T>
    [[nodiscard]] bool
    resize(const Slice<T> buf, const u64 new_len) {
        return vtable.resize_fn(ctx, buf.ptr, buf.len * sizeof(T), new_len * sizeof(T), alignof(T));
    }

    template <typename T>
    void
    free(const Slice<T> buf) {
        vtable.free_fn(ctx, buf.ptr, buf.len * sizeof(T), alignof(T));
    }

    [[nodiscard]] bool
    raw_alloc(const u64 len, const u64 alignment, Slice<u8>* out_block) {
        return vtable.alloc_fn(ctx, len, alignment, out_block);
    }
};

template <typename T>
constexpr T
align_down(const T addr, const T alignment) {
    return addr & ~(alignment - 1);
}

template <typename T>
constexpr T
align_up(const T addr, const T alignment) {
    const T mask = alignment - 1;
    return (addr + mask) & ~mask;
}

template <typename T>
void
zero(const Slice<T> slice) {
    u8* it = (u8*)slice.ptr;
    for (u64 idx = 0; idx < sizeof(T) * slice.len; ++idx) {
        it[idx] = 0;
    }
}

template <typename T>
void
copy(const Slice<T> dst, const Slice<T> src) {
    assert(dst.len == src.len);
    for (u64 idx = 0; idx < dst.len; ++idx) {
        dst.ptr[idx] = src.ptr[idx];
    }
}

} // namespace mem
} // namespace mksv

using Str = mksv::mem::Slice<u8>;

Str
str(const char* str);
