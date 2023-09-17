#include "heap.hpp"

#include "ctx.hpp"
#include "math.hpp"

#if OS_WINDOWS
#include <windows.h>
#endif

namespace mksv {
namespace heap {

static struct Win32Ctx {
} win32_ctx;

static constexpr u64
win32_page_granularity() {
    return math::kilo_bytes(4);
}

static void*
win32_allocate(void* ctx, const u64 size, const u64 alignment) {
    (void)ctx;

    u64 aligned_size = mem::align_up<u64>(size, win32_page_granularity());
    aligned_size = mem::align_up<u64>(aligned_size, alignment);

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
#if OS_WINDOWS
    return {
        .ctx = &win32_ctx,
        .vtable = {
            .alloc_fn = &win32_allocate,
            .resize_fn = &win32_resize,
            .free_fn = &win32_free,
        },
    };
#elif OS_LINUX
#elif OS_MACOS
#else
#error "Unsupported OS"
#endif
}

static void*
arena_alloc(void* ctx, const u64 size, const u64 alignment) {
    ArenaAllocator* context = (ArenaAllocator*)ctx;

    const u64 node_size = sizeof(*context->stack.head);
    const u64 aligned_node_size = mem::align_up(node_size, alignment);
    const u64 aligned_size = mem::align_up(size, alignment);
    const mem::Span<u8> block =
        context->inner_allocator.alloc<u8>(aligned_size + aligned_node_size);

    ArenaAllocator::Node* node = (ArenaAllocator::Node*)block.ptr;
    node->data = { .span = block };
    context->stack.append_node(node);

    return block.ptr + aligned_node_size;
}

static bool
arena_resize(
    void* ctx,
    void* ptr,
    const u64 old_size,
    const u64 new_size,
    const u64 alignment
) {
    ArenaAllocator* context = (ArenaAllocator*)ctx;
    (void)context;
    (void)ptr;
    (void)old_size;
    (void)new_size;
    (void)alignment;
    return false;
}

static void
arena_free(void* ctx, void* ptr, const u64 size, const u64 alignment) {
    (void)ctx;
    (void)ptr;
    (void)size;
    (void)alignment;
}

ArenaAllocator
ArenaAllocator::init(const mem::Allocator inner) {
    return {
        .inner_allocator = inner,
    };
}

mem::Allocator
ArenaAllocator::allocator() const {
    return {
        .ctx = (void*)this,
        .vtable = {
            .alloc_fn = &arena_alloc,
            .resize_fn = &arena_resize,
            .free_fn = &arena_free,
        },
    };
}

void
ArenaAllocator::deinit() {
    auto ptr = stack.head;
    while (ptr) {
        const auto tmp = ptr->next;
        ArenaAllocator::Node node;
        assert(stack.pop_front(&node));
        inner_allocator.free(node.data.span);
        ptr = tmp;
    }

}

} // namespace heap
} // namespace mksv
