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

static struct OsCtx {
} os_ctx;

static bool
os_allocate(void* ctx, const u64 size, const u64 alignment, mem::Slice<u8>* out_block) {
    (void)ctx;

#if OS_WINDOWS
    u64 aligned_size = mem::align_up<u64>(size, os_page_granularity());
    aligned_size = mem::align_up<u64>(aligned_size, alignment);

    void* block = VirtualAlloc(nullptr, aligned_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (block == nullptr) return false;

    *out_block = mem::Slice<u8>{ (u8*)block, aligned_size };
    return true;
#elif OS_MACOS
    u64 aligned_size = mem::align_up<u64>(size, os_page_granularity());
    aligned_size = mem::align_up<u64>(aligned_size, alignment);

    void* block =
        mmap(nullptr, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (block == MAP_FAILED) return false;

    *out_block = mem::Slice<u8>{ (u8*)block, aligned_size };
    return true;
#else
#error "Unsupported OS"
#endif
}

static bool
os_resize(void* ctx, void* ptr, const u64 old_size, const u64 new_size, const u64 alignment) {
    (void)ctx;
    (void)ptr;
    (void)old_size;
    (void)new_size;
    (void)alignment;

    return false;
}

static void
os_free(void* ctx, void* ptr, const u64 size, const u64 alignment) {
    (void)ctx;
    (void)alignment;

#if OS_WINDOWS
    VirtualFree(ptr, 0, MEM_RELEASE);
#elif OS_MACOS
    munmap(ptr, size);
#else
#error "Unsupported OS"
#endif
}

mem::Allocator
system_allocator() {
    return {
        .ctx = &os_ctx,
        .vtable = {
            .alloc_fn = &os_allocate,
            .resize_fn = &os_resize,
            .free_fn = &os_free,
        },
    };
}

static bool
arena_alloc(void* ctx, const u64 size, const u64 alignment, mem::Slice<u8>* out_block) {
    ArenaAllocator* context = (ArenaAllocator*)ctx;

    const u64 aligned_size = mem::align_up(size, alignment);

    if (context->stack.head) {
        const auto head = context->stack.head;
        const u64 aligned_index = mem::align_up(context->end_idx, alignment);
        const u64 size_left = head->data.len - aligned_index;
        if (size_left >= aligned_size) {
            *out_block = mem::Slice<u8>{ head->data.ptr + aligned_index, size };
            context->end_idx = aligned_index;
            context->end_idx += aligned_size;
            return true;
        }
    }

    const u64 aligned_node_size = mem::align_up(ArenaAllocator::NODE_SIZE, alignment);

    mem::Slice<u8> block = {};
    if (!context->inner_allocator.raw_alloc(aligned_size + aligned_node_size, alignment, &block))
        return false;

    ArenaAllocator::Node* node = (ArenaAllocator::Node*)block.ptr;
    node->data = block;
    context->stack.prepend_node(node);
    context->end_idx = aligned_node_size + aligned_size;

    *out_block = mem::Slice<u8>{ block.ptr + aligned_node_size, size };

    return true;
}

static bool
arena_resize(void* ctx, void* ptr, const u64 old_size, const u64 new_size, const u64 alignment) {
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
