#include "file.hpp"

#include "ctx.hpp"

#if OS_MACOS
#include <unistd.h>
#endif

namespace mksv {
namespace io {

bool
read_file(const Str filename, Str* out_str) {
#if OS_WINDOWS
#elif OS_MACOS
    (void)filename;
    (void)out_str;
    return false;
#endif
}

} // namespace io
} // namespace mksv
