#include "file.hpp"

#include "ctx.hpp"
#include "mem.hpp"
#include "utils.hpp"

#if OS_WINDOWS
#include <windows.h>
#endif

#if OS_MACOS
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
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
    BOOL success = ReadFile(handle, (LPVOID)file_content.ptr, size32, &bytes_read, nullptr);
    if (!success || bytes_read != size32) {
        allocator.free(file_content);
        return false;
    }

    *out_str = file_content;

    return true;
#elif OS_MACOS
    char c_str[PATH_MAX] = {};
    mem::copy({ .ptr = (u8*)c_str, .len = filename.len }, filename);

    const int fd = open(c_str, O_RDONLY);
    if (fd < 0) return false;
    defer(close(fd));

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) return false;

    Str file_content = {};
    if (!allocator.alloc((u64)file_stat.st_size, &file_content)) return false;

    const ssize_t bytes_read = read(fd, file_content.ptr, file_content.len);
    if (bytes_read < 0) {
        allocator.free(file_content);
        return false;
    }

    *out_str = file_content;

    return true;
#elif OS_LINUX
#else
#error "Unsupported OS"
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
    BOOL success = WriteFile(handle, (LPCVOID)buffer.ptr, size32, &bytes_written, nullptr);
    if (!success || bytes_written != size32) return false;

    return true;
#elif OS_MACOS
    char c_str[PATH_MAX] = {};
    mem::copy({ .ptr = (u8*)c_str, .len = filename.len }, filename);

    const int fd = open(c_str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) return false;
    defer(close(fd));

    const ssize_t bytes_written = write(fd, (void*)buffer.ptr, buffer.len);
    if (bytes_written < 0 || bytes_written != (ssize_t)buffer.len) return false;

    return true;
#elif OS_LINUX
#else
#error "Unsupported OS"
#endif
}

[[nodiscard]] bool
write_stdout(const Str buffer) {
#if OS_WINDOWS
#elif OS_MACOS
    const ssize_t bytes_written = write(STDOUT_FILENO, (void*)buffer.ptr, buffer.len);
    if (bytes_written < 0 || bytes_written != (ssize_t)buffer.len) return false;

    return true;
#elif OS_LINUX
#else
#error "Unsupported OS"
#endif
}

[[nodiscard]] bool
write_stderr(const Str buffer) {
#if OS_WINDOWS
#elif OS_MACOS
    const ssize_t bytes_written = write(STDERR_FILENO, (void*)buffer.ptr, buffer.len);
    if (bytes_written < 0 || bytes_written != (ssize_t)buffer.len) return false;

    return true;
#elif OS_LINUX
#else
#error "Unsupported OS"
#endif
}
} // namespace io
} // namespace mksv
