#include "assert.hpp"

#include "io.hpp"

namespace mksv {

static u64
numlen(u64 num) {
    u64 len = 0;

    while (num / 10 != 0) {
        num /= 10;
        ++len;
    }

    return len + 1;
}

static Str
itoa(const Str buffer, u64 num) {
    u64 len = numlen(num);
    u64 ret_len = (u64)len;

    if (ret_len > buffer.len) return Str::null();

    while (len > 0) {
        u64 n = num % 10;
        --len;
        buffer.ptr[len] = '0' + n;
        num /= 10;
    }

    return Str{ buffer.ptr, ret_len };
}

static u64
write_str(const Str buffer, const Str s) {
    u64 idx = 0;
    while (idx < s.len) {
        buffer.ptr[idx] = s.ptr[idx];
        ++idx;
    }
    return idx;
}

void
assert_message(const char* expr, const char* file, const u64 line_number) {
    static u8 buffer[1024];

    Str buf_str = str(buffer);
    u64 idx = 0;

    u8 num_buf[32];
    const Str num = itoa(str(num_buf), line_number);

    idx += write_str(buf_str, "Assertion failed: \"");
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, expr);
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, "\" (");
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, file);
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, ":");
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, num);
    idx += write_str(Str{ buf_str.ptr + idx, buf_str.len - idx }, ")\n");
    (void)io::write_stderr(Str{ buffer, idx });
    *(volatile char*)0 = 1;
}

} // namespace mksv
