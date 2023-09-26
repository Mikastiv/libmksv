#pragma once

#include "assert.hpp"
#include "math.hpp"
#include "types.hpp"

namespace mksv {
namespace mem {

template <typename T>
struct Slice {
    T* ptr;
    u64 len;

    constexpr Slice
    at(const u64 index) const {
        assert(index < len);
        return {
            .ptr = ptr + index,
            .len = len - index,
        };
    }

    constexpr Slice
    sub(const u64 start, const u64 end) const {
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
constexpr bool
is_delimiter(const Slice<T> slice, const Slice<T> delimiter, const u64 index) {
    if (index + delimiter.len > slice.len) return false;
    return equal(slice.sub(index, index + delimiter.len), delimiter);
}

template <typename T>
struct TokenIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;

    [[nodiscard]] constexpr bool
    next(Slice<T>* value) {
        while (index < buffer.len && is_delimiter(buffer, delimiter, index)) {
            index += delimiter.len;
        }

        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(buffer, delimiter, end)) ++end;

        index += end - start;

        *value = buffer.sub(start, end);

        return true;
    }
};

template <typename T>
TokenIter<T>
tokenize(const Slice<T> slice, const Slice<T> delimiter) {
    return {
        .buffer = slice,
        .delimiter = delimiter,
        .index = 0,
    };
}

template <typename T>
struct SplitIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;

    [[nodiscard]] constexpr bool
    next(Slice<T>* value) {
        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(buffer, delimiter, end)) ++end;

        index += end - start;
        index = math::min(buffer.len, index + 1);

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
    alloc(const u64 len, Slice<T>* out_block) const {
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
    resize(const Slice<T> buf, const u64 new_len) const {
        return vtable.resize_fn(ctx, buf.ptr, buf.len * sizeof(T), new_len * sizeof(T), alignof(T));
    }

    template <typename T>
    void
    free(const Slice<T> buf) const {
        vtable.free_fn(ctx, buf.ptr, buf.len * sizeof(T), alignof(T));
    }

    [[nodiscard]] bool
    raw_alloc(const u64 len, const u64 alignment, Slice<u8>* out_block) const {
        return vtable.alloc_fn(ctx, len, alignment, out_block);
    }
};

// TODO: check alignment is power of 2
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
constexpr void
zero(const Slice<T> slice) {
    u8* it = (u8*)slice.ptr;
    for (u64 idx = 0; idx < sizeof(T) * slice.len; ++idx) {
        it[idx] = 0;
    }
}

template <typename T>
constexpr void
copy(const Slice<T> dst, const Slice<T> src) {
    assert(dst.len == src.len);
    for (u64 idx = 0; idx < dst.len; ++idx) {
        dst.ptr[idx] = src.ptr[idx];
    }
}

template <typename T>
bool
join(const Allocator allocator, Slice<T>* dst, const Slice<T> a, const Slice<T> b) {
    if (!allocator.alloc(a.len + b.len, dst)) return false;
    copy({ .ptr = dst->ptr, .len = a.len }, a);
    copy({ .ptr = dst->ptr + a.len, .len = b.len }, b);
    return true;
}

template <typename T>
[[nodiscard]] constexpr bool
find(const Slice<T> haystack, const Slice<T> needle, u64* out_idx) {
    if (needle.len > haystack.len) return false;
    if (haystack.len == 0 || needle.len == 0) return false;
    if (equal(haystack, needle)) {
        *out_idx = 0;
        return true;
    }

    for (u64 idx = 0; idx < haystack.len - needle.len; ++idx) {
        if (equal({ .ptr = haystack.ptr + idx, .len = needle.len }, needle)) {
            *out_idx = idx;
            return true;
        }
    }

    return false;
}

} // namespace mem

} // namespace mksv

using Str = mksv::mem::Slice<u8>;

constexpr u64
str_len(const char* str) {
    u64 len = 0;
    while (str[len]) len++;
    return len;
}

inline Str
str(const char* str) {
    return {
        .ptr = (u8*)str,
        .len = str_len(str),
    };
}

[[nodiscard]] bool
dupe_z(const mksv::mem::Allocator allocator, const Str string, Str* out_string);
