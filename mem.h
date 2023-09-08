#pragma once

#include "types.h"

typedef struct Str {
    u8* ptr;
    u64 len;
} Str;

typedef struct SplitIter {
    Str buffer;
    Str delimiter;
    u64 index;
} SplitIter;

Str
str_at(const Str str, const u64 index);

Str
str_sub(const Str str, const u64 start, const u64 end);

bool
str_equal(const Str a, const Str b);

bool
str_is_delimiter(const SplitIter it, const u64 index);

bool
iter_next(SplitIter* it, Str* value);

SplitIter
str_split(const Str str, const Str delimiter);

Str
str(const char* str);
