#include "hash.hpp"
#include "bit.hpp"

namespace mksv {
namespace hash {

constexpr u64 HASH_SEED = 0;
constexpr u64 K = 0x517CC1B727220A95;

// Firefox FxHasher
static constexpr u64
hash_combine(const u64 seed, const u64 w) {
    return (bit::rotate_left(seed, 5) ^ w) * K;
}

u64
hash(const mem::Slice<u8> s) {
    u64 hash_value = HASH_SEED;
    const u64 chunks_count = s.len / sizeof(u64);
    const u64 leftover = s.len - (chunks_count * sizeof(u64));

    mem::Slice<u64> chunks = { (u64*)s.ptr, chunks_count };
    for (const auto c : chunks) {
        hash_value = hash_combine(hash_value, c);
    }

    for (u64 i = s.len - leftover; i < s.len; ++i) {
        hash_value = hash_combine(hash_value, s[i]);
    }

    return hash_value;
}

} // namespace hash
} // namespace mksv
