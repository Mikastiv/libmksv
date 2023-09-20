#include "file.hpp"

#include "ctx.hpp"
#include "mem.hpp"
#include "utils.hpp"

#if OS_WINDOWS
#include <windows.h>
#endif

#if OS_MACOS
#include <unistd.h>
#endif

namespace mksv {
namespace io {

bool
read_file(mem::Allocator allocator, const Str filename, Str* out_str) {
#if OS_WINDOWS
    char c_str[MAX_PATH] = {};
    mem::copy({ .ptr = (u8*)c_str, .len = filename.len }, filename);

    HANDLE handle = CreateFileA(
        c_str,
        FILE_GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (handle == INVALID_HANDLE_VALUE) return false;
    defer(CloseHandle(handle));

    LARGE_INTEGER filesize = {};
    if (FAILED(GetFileSizeEx(handle, &filesize))) return false;

    mem::Slice<u8> file_content = {};
    if (!allocator.alloc((u64)filesize.QuadPart, &file_content)) return false;

    DWORD bytes_read = 0;
    const DWORD size32 = filesize.QuadPart;
    BOOL success = ReadFile(
        handle,
        (LPVOID)file_content.ptr,
        size32,
        &bytes_read,
        nullptr
    );
    if (!success || bytes_read != size32) {
        allocator.free(file_content);
        return false;
    }

    *out_str = file_content;

    return true;
#elif OS_MACOS
    (void)filename;
    (void)out_str;
    return false;
#endif
}

bool
write_file(const Str filename, const Str buffer) {
#if OS_WINDOWS
    char c_str[MAX_PATH] = {};
    mem::copy({ .ptr = (u8*)c_str, .len = filename.len }, filename);

    HANDLE handle = CreateFileA(
        c_str,
        FILE_GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (handle == INVALID_HANDLE_VALUE) return false;
    defer(CloseHandle(handle));

    DWORD bytes_written = 0;
    const DWORD size32 = buffer.len;
    BOOL success =
        WriteFile(handle, (LPCVOID)buffer.ptr, size32, &bytes_written, nullptr);
    if (!success || bytes_written != size32) return false;

    return true;
#endif
}

} // namespace io
} // namespace mksv
