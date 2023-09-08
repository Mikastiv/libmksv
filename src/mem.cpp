#include "mem.hpp"

namespace mem {}

Str
str(const char* str) {
    return {
        .ptr = (u8*)str,
        .len = strlen(str),
    };
}
