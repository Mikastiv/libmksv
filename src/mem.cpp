#include "mem.hpp"

namespace mksv {
namespace mem {}

} // namespace mksv

bool
dupe_z(const mksv::mem::Allocator allocator, const Str string, Str* out_string) {
    if (!allocator.alloc(string.len + 1, out_string)) return false;
    mksv::mem::copy(Str{ out_string->ptr, string.len }, string);
    out_string->ptr[string.len] = 0;
    return true;
}
