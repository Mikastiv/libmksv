#pragma once

#define assert(expr)                                                           \
    if (!(expr)) {                                                             \
        *(volatile char*)0 = 0;                                                         \
    }
