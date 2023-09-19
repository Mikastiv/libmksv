#pragma once

#include "assert.hpp"
#include "types.hpp"

namespace mksv {
namespace mem {

template <typename T>
struct Span {
    T* ptr;
    u64 len;

    static constexpr Span
    null() {
        return {
            .ptr = nullptr,
            .len = 0,
        };
    }

    constexpr Span
    at(const u64 index) {
        assert(index < len);
        return {
            .ptr = ptr + index,
            .len = len - index,
        };
    }

    constexpr Span
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
equal(const Span<T> a, const Span<T> b) {
    if (a.len != b.len) return false;
    if (a.ptr == b.ptr) return true;

    for (u64 i = 0; i < a.len; ++i) {
        if (a.ptr[i] != b.ptr[i]) return false;
    }

    return true;
}

template <typename T>
struct SplitIter {
    Span<T> buffer;
    Span<T> delimiter;
    u64 index;

    constexpr bool
    is_delimiter(const u64 index) {
        if (index + delimiter.len > buffer.len) return false;
        return equal(sub(buffer, index, index + delimiter.len), delimiter);
    }

    constexpr bool
    next(Span<T>* value) {
        while (index < buffer.len && is_delimiter(index)) {
            index += delimiter.len;
        }

        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(end)) ++end;

        index += end - start;

        *value = sub(buffer, start, end);

        return true;
    }
};

template <typename T>
SplitIter<T>
split(const Span<T> span, const Span<T> delimiter) {
    return {
        .buffer = span,
        .delimiter = delimiter,
        .index = 0,
    };
}

typedef mem::Span<u8> (*AllocFn)(void*, const u64, const u64);
typedef bool (*ResizeFn)(
    void* ctx,
    void* ptr,
    const u64 old_size,
    const u64 new_size,
    const u64 alignment
);
typedef void (*FreeFn)(
    void* ctx,
    void* ptr,
    const u64 size,
    const u64 alignment
);

struct Allocator {
    void* ctx;

    struct VTable {
        AllocFn alloc_fn;
        ResizeFn resize_fn;
        FreeFn free_fn;
    } vtable;

    template <typename T>
    Span<T>
    alloc(const u64 len) {
        const auto block = vtable.alloc_fn(ctx, len * sizeof(T), alignof(T));
        if (block.ptr == nullptr) return mem::Span<T>::null();
        return {
            .ptr = (T*)block.ptr,
            .len = len,
        };
    }

    template <typename T>
    bool
    resize(const Span<T> buf, const u64 new_len) {
        return vtable.resize_fn(
            ctx,
            buf.ptr,
            buf.len * sizeof(T),
            new_len * sizeof(T),
            alignof(T)
        );
    }

    template <typename T>
    void
    free(const Span<T> buf) {
        vtable.free_fn(ctx, buf.ptr, buf.len * sizeof(T), alignof(T));
    }

    Span<u8>
    raw_alloc(const u64 len, const u64 alignment) {
        return vtable.alloc_fn(ctx, len, alignment);
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
zero(const Span<T> span) {
    u8* it = (u8*)span.ptr;
    for (u64 idx = 0; idx < sizeof(T) * span.len; ++idx) {
        it[idx] = 0;
    }
}

template <typename T>
void
copy(const Span<T> dst, const Span<T> src) {
    assert(dst.len == src.len);
    for (u64 idx = 0; idx < dst.len; ++idx) {
        dst.ptr[idx] = src.ptr[idx];
    }
}

} // namespace mem
} // namespace mksv

typedef mksv::mem::Span<u8> Str;

Str
str(const char* str);
