#include "mem.hpp"

namespace mksv {
namespace mem {}

} // namespace mksv

bool
dupe_z(const mksv::mem::Allocator allocator, const Str string, Str* out_string) {
    if (!allocator.alloc(string.len + 1, out_string)) return false;
    mksv::mem::copy({ .ptr = out_string->ptr, .len = string.len }, string);
    out_string->ptr[out_string->len] = 0;
    return true;
}
