#include "mem.h"

#include <assert.h>
#include <string.h>

Str
str_at(const Str str, const u64 index) {
    assert(index < str.len);
    return (Str){
        .ptr = str.ptr + index,
        .len = str.len - index,
    };
}

Str
str_sub(const Str str, const u64 start, const u64 end) {
    assert(end >= start);
    assert(end <= str.len);
    return (Str){
        .ptr = str.ptr + start,
        .len = end - start,
    };
}

bool
str_equal(const Str a, const Str b) {
    if (a.len != b.len) return false;
    if (a.ptr == b.ptr) return true;

    for (u64 i = 0; i < a.len; ++i) {
        if (a.ptr[i] != b.ptr[i]) return false;
    }

    return true;
}

bool
str_is_delimiter(const SplitIter it, const u64 index) {
    if (index + it.delimiter.len > it.buffer.len) return false;
    return str_equal(str_sub(it.buffer, index, index + it.delimiter.len), it.delimiter);
}

bool
iter_next(SplitIter* it, Str* value) {
    while (it->index < it->buffer.len && str_is_delimiter(*it, it->index)) {
        it->index += it->delimiter.len;
    }

    if (it->index == it->buffer.len) return false;

    const u64 start = it->index;
    u64 end = start;
    while (end < it->buffer.len && !str_is_delimiter(*it, end)) ++end;

    it->index += end - start;

    *value = str_sub(it->buffer, start, end);

    return true;
}

SplitIter
str_split(const Str str, const Str delimiter) {
    return (SplitIter){
        .buffer = str,
        .delimiter = delimiter,
        .index = 0,
    };
}

Str
str(const char* str) {
    return (Str){
        .ptr = (u8*)str,
        .len = strlen(str),
    };
}
