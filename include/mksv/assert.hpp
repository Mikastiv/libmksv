#pragma once

#define assert(expr)                                                           \
    if (!(expr)) {                                                             \
        *(char*)0 = 0;                                                         \
    }
