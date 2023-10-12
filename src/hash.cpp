#include "hash.hpp"

namespace mksv {
namespace hash {

constexpr u64 HASH_SEED = 0;

u64
hash(const mem::Slice<u8> s) {
    u64 hash_value = HASH_SEED;
    for (const auto byte : s) {
        hash_value = hash_combine(hash_value, byte);
    }
    return hash_value;
}

u64
hash_combine(const u64 seed, const u8 byte) {
    const u64 value = (u32)byte + 0x9E3779B9 + (seed << 6) + (seed << 2);
    return seed ^ value;
}

} // namespace hash
} // namespace mksv
