#pragma once

#include "assert.hpp"
#include "types.hpp"

namespace mem {

template <typename T>
struct Span {
    T* ptr;
    u64 len;

    Span
    at(const u64 index) {
        assert(index < len);
        return {
            .ptr = ptr + index,
            .len = len - index,
        };
    }

    Span
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
bool
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

    bool
    is_delimiter(const u64 index) {
        if (index + delimiter.len > buffer.len) return false;
        return equal(sub(buffer, index, index + delimiter.len), delimiter);
    }

    bool
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

} // namespace mem

typedef mem::Span<u8> Str;

Str
str(const char* str);
