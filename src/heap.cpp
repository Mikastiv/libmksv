#include "heap.hpp"

#include "math.hpp"
#include "mem.hpp"
#include "singly_linked_list.hpp"

#include <windows.h>

namespace mksv {
namespace heap {

static struct Win32Ctx {

} win32_ctx;

static void*
win32_allocate(void* ctx, const u64 size, const u64 alignment) {
    (void)ctx;

    const u64 aligned_size = mem::align_up<u64>(size, alignment);
    return VirtualAlloc(
        0,
        aligned_size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
}

static bool
win32_resize(
    void* ctx,
    void* ptr,
    const u64 old_size,
    const u64 new_size,
    const u64 alignment
) {
    (void)ctx;
    (void)ptr;
    (void)old_size;
    (void)new_size;
    (void)alignment;

    return false;
}

static void
win32_free(void* ctx, void* ptr, const u64 size, const u64 alignment) {
    (void)ctx;
    (void)size;
    (void)alignment;

    VirtualFree(ptr, 0, MEM_RELEASE);
}

mem::Allocator
system_allocator() {
    return {
        .ctx = &win32_ctx,
        .vtable = {
            .alloc_fn = &win32_allocate,
            .resize_fn = &win32_resize,
            .free_fn = &win32_free,
        },
    };
}

ArenaAllocator
ArenaAllocator::init(const mem::Allocator inner) {
    return {
        .inner_allocator = inner,
    };
}

void
deinit() {
}

} // namespace heap
} // namespace mksv
