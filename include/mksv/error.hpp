#pragma once

#include "fmt.hpp"
#include "io.hpp"
#include "mem.hpp"

#define ALLOC_FAILED mksv::err::error_with_info("Memory allocation failed", __FILE__, __LINE__)
#define ERROR(msg) mksv::err::error_with_info(msg, __FILE__, __LINE__)

namespace mksv {
namespace err {

inline bool
error(const Str message) {
    io::write_stderr(message);
    return false;
}

template <typename T, typename... Args>
inline bool
error(const Str fmt, T value, Args... args) {
    fmt::print_stderr(fmt, value, args...);
    return false;
}

inline bool
error_with_info(const Str message, const Str file, const u64 line) {
    return error("{s} ({s}:{u64})\n", message, file, line);
}

} // namespace err
} // namespace mksv
