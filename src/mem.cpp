#include "mem.hpp"

#include <string.h>

namespace mem {}

// TODO: Remove strlen with own implementation
Str
str(const char* str) {
    return {
        .ptr = (u8*)str,
        .len = strlen(str),
    };
}
