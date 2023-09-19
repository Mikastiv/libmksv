#include "heap.hpp"

#include "ctx.hpp"
#include "math.hpp"
#include "mem.hpp"

#if OS_WINDOWS
#include <windows.h>
#endif
#if OS_MACOS
#include <sys/mman.h>
#endif

namespace mksv {
namespace heap {

static constexpr u64
os_page_granularity() {
#if OS_WINDOWS | OS_MACOS | OS_LINUX
    return math::kilo_bytes(4);
#endif
}

#if OS_WINDOWS
static struct Win32Ctx {
} win32_ctx;

static mem::Slice<u8>
win32_allocate(void* ctx, const u64 size, const u64 alignment) {
    (void)ctx;

    u64 aligned_size = mem::align_up<u64>(size, os_page_granularity());
    aligned_size = mem::align_up<u64>(aligned_size, alignment);

    void* block =
        VirtualAlloc(0, aligned_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (block == nullptr) return mem::Slice<u8>::null();

    return {
        .ptr = (u8*)block,
        .len = aligned_size,
    };
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
#endif

#if OS_MACOS
static struct MacosCtx {
} macos_ctx;

static mem::Slice<u8>
macos_allocate(void* ctx, const u64 size, const u64 alignment) {
    (void)ctx;

    u64 aligned_size = mem::align_up<u64>(size, os_page_granularity());
    aligned_size = mem::align_up<u64>(aligned_size, alignment);

    void* block = mmap(
        nullptr,
        aligned_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON,
        -1,
        0
    );
    if (block == MAP_FAILED) return mem::Slice<u8>::null();

    return { .ptr = (u8*)block, .len = aligned_size };
}

static bool
macos_resize(
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
macos_free(void* ctx, void* ptr, const u64 size, const u64 alignment) {
    (void)ctx;
    (void)alignment;

    munmap(ptr, size);
}
#endif

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
    return {
                .ctx = &macos_ctx,
        .vtable = {
            .alloc_fn = &macos_allocate,
            .resize_fn = &macos_resize,
            .free_fn = &macos_free,
        },
    };
#else
#error "Unsupported OS"
#endif
}

static mem::Slice<u8>
arena_alloc(void* ctx, const u64 size, const u64 alignment) {
    ArenaAllocator* context = (ArenaAllocator*)ctx;

    const u64 aligned_size = mem::align_up(size, alignment);

    if (context->stack.head) {
        const auto head = context->stack.head;
        const u64 size_left = head->data.len - context->end_idx;
        if (size_left >= aligned_size) {
            const mem::Slice<u8> block = {
                .ptr = head->data.ptr + context->end_idx,
                .len = size,
            };
            context->end_idx += aligned_size;
            return block;
        }
    }

    const u64 aligned_node_size =
        mem::align_up(ArenaAllocator::NODE_SIZE, alignment);

    const auto block = context->inner_allocator.raw_alloc(
        aligned_size + aligned_node_size,
        alignment
    );
    if (block.ptr == nullptr) return mem::Slice<u8>::null();

    ArenaAllocator::Node* node = (ArenaAllocator::Node*)block.ptr;
    node->data = block;
    context->stack.append_node(node);
    context->end_idx = aligned_node_size + aligned_size;

    return { .ptr = block.ptr + aligned_node_size, .len = size };
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
        .end_idx = 0,
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
        inner_allocator.free(node.data);
        ptr = tmp;
    }
}

} // namespace heap
} // namespace mksv
