#pragma once

#include "assert.hpp"
#include "math.hpp"
#include "type_traits.hpp"
#include "types.hpp"

namespace mksv {

constexpr u64
str_len(const char* str) {
    u64 len = 0;
    while (str[len]) len++;
    return len;
}

namespace mem {

template <typename T>
struct Slice {
    T* ptr = nullptr;
    u64 len = 0;

    static constexpr Slice
    null() {
        return Slice{ nullptr, 0 };
    }

    constexpr Slice() {
    }

    constexpr Slice(T* ptr, u64 len)
        : ptr{ ptr },
          len{ len } {
    }

    // Conversion from string litteral to Slice
    constexpr Slice(const char* s)
        requires(traits::is_same_v<u8, T>)
        : ptr{ (u8*)s },
          len{ str_len(s) } {
    }

    constexpr Slice
    at(const u64 index) const {
        assert(index < len);
        return Slice{ ptr + index, len - index };
    }

    constexpr Slice
    sub(const u64 start, const u64 end) const {
        assert(end >= start);
        assert(end <= len);
        return Slice{ ptr + start, end - start };
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
ends_with(const Slice<T> a, const Slice<T> b) {
    if (a.len < b.len) return false;
    return equal(Slice<T>(a.ptr + a.len - b.len, b.len), b);
}

template <typename T>
constexpr bool
is_delimiter(const Slice<T> slice, const Slice<T> delimiter, const u64 index, const bool any) {
    const u64 len = any ? 1 : delimiter.len;
    if (index + len > slice.len) return false;

    // match whole delimiter
    if (!any) return equal(slice.sub(index, index + delimiter.len), delimiter);

    // match any character
    for (u64 i = 0; i < delimiter.len; ++i) {
        if (delimiter.ptr[i] == *slice.sub(index, index + 1).ptr) return true;
    }
    return false;
}

template <typename T>
struct TokenIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;
    bool any;

    [[nodiscard]] constexpr bool
    next(Slice<T>* value) {
        const u64 len = any ? 1 : delimiter.len;

        while (index < buffer.len && is_delimiter(buffer, delimiter, index, any)) {
            index += len;
        }

        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(buffer, delimiter, end, any)) ++end;

        index += end - start;

        *value = buffer.sub(start, end);

        return true;
    }
};

template <typename T>
TokenIter<T>
tokenize(const Slice<T> slice, const Slice<T> delimiter, const bool any) {
    return {
        .buffer = slice,
        .delimiter = delimiter,
        .index = 0,
        .any = any,
    };
}

template <typename T>
[[nodiscard]] constexpr bool
tokenize_once(const Slice<T> slice, const Slice<T> delimiter, const bool any, Slice<T>* token) {
    auto it = tokenize(slice, delimiter, any);
    return it.next(token);
}

template <typename T>
struct RevTokenIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;
    bool any;
    bool done = false;

    [[nodiscard]] constexpr bool
    next(Slice<T>* value) {
        const u64 len = any ? 1 : delimiter.len;

        if (done) return false;

        u64 end = index;

        while (index != 0 && is_delimiter(buffer, delimiter, index, any)) {
            if (index < len)
                index = 0;
            else
                index -= len;
        }

        if (end != index) end = index + 1;

        u64 start = index;
        while (start != 0 && !is_delimiter(buffer, delimiter, start, any)) --start;

        index = start;
        if (index == 0) done = true;

        *value = buffer.sub(start + (start == 0 ? 0 : len), end);

        return true;
    }
};

template <typename T>
RevTokenIter<T>
reverse_tokenize(const Slice<T> slice, const Slice<T> delimiter, const bool any) {
    return {
        .buffer = slice,
        .delimiter = delimiter,
        .index = slice.len,
        .any = any,
    };
}

template <typename T>
[[nodiscard]] constexpr bool
reverse_tokenize_once(
    const Slice<T> slice,
    const Slice<T> delimiter,
    const bool any,
    Slice<T>* token
) {
    auto it = reverse_tokenize(slice, delimiter, any);
    return it.next(token);
}

template <typename T>
struct SplitIter {
    Slice<T> buffer;
    Slice<T> delimiter;
    u64 index;
    bool any;

    [[nodiscard]] constexpr bool
    next(Slice<T>* value) {
        const u64 len = any ? 1 : delimiter.len;

        if (index == buffer.len) return false;

        const u64 start = index;
        u64 end = start;
        while (end < buffer.len && !is_delimiter(buffer, delimiter, end, any)) ++end;

        index += end - start;
        index = math::min(buffer.len, index + len);

        *value = buffer.sub(start, end);

        return true;
    }
};

template <typename T>
SplitIter<T>
split(const Slice<T> slice, const Slice<T> delimiter, const bool any) {
    return {
        .buffer = slice,
        .delimiter = delimiter,
        .index = 0,
        .any = any,
    };
}

template <typename T>
[[nodiscard]] constexpr bool
split_once(const Slice<T> slice, const Slice<T> delimiter, const bool any, Slice<T>* token) {
    auto it = split(slice, delimiter, any);
    return it.next(token);
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

        *out_block = Slice{ (T*)block.ptr, len };

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

    [[nodiscard]] inline bool
    raw_alloc(const u64 len, const u64 alignment, Slice<u8>* out_block) const {
        return vtable.alloc_fn(ctx, len, alignment, out_block);
    }
};

template <typename T>
constexpr T
align_down(const T addr, const T alignment) {
    assert(alignment != 0);
    assert((alignment & (alignment - 1)) == 0);
    return addr & ~(alignment - 1);
}

template <typename T>
constexpr T
align_up(const T addr, const T alignment) {
    assert(alignment != 0);
    assert((alignment & (alignment - 1)) == 0);
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
[[nodiscard]] bool
join(const Allocator allocator, Slice<T>* dst, const Slice<T> a, const Slice<T> b) {
    if (!allocator.alloc(a.len + b.len, dst)) return false;
    copy(Slice{ dst->ptr, a.len }, a);
    copy(Slice{ dst->ptr + a.len, b.len }, b);
    return true;
}

template <typename T>
constexpr bool
find(const Slice<T> haystack, const Slice<T> needle, u64* out_idx) {
    if (needle.len > haystack.len) return false;
    if (haystack.len == 0 || needle.len == 0) return false;
    if (equal(haystack, needle)) {
        *out_idx = 0;
        return true;
    }

    for (u64 idx = 0; idx < haystack.len - needle.len; ++idx) {
        if (equal(Slice{ haystack.ptr + idx, needle.len }, needle)) {
            *out_idx = idx;
            return true;
        }
    }

    return false;
}

} // namespace mem

} // namespace mksv

using Str = mksv::mem::Slice<u8>;

template <u64 size>
inline Str
str(u8 (&array)[size]) {
    return Str{ array, size };
}

[[nodiscard]] bool
dupe_z(const mksv::mem::Allocator allocator, const Str string, Str* out_string);
