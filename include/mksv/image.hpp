#pragma once

#include "mem.hpp"

namespace mksv {

struct Image {
    u64 width;
    u64 height;
    u64 bpp;
    mem::Slice<u32> pixels;
};

namespace img {

[[nodiscard]] bool
load_tga(const mem::Allocator allocator, const Str filename, Image* out_image);

[[nodiscard]] bool
load_bmp(const mem::Allocator allocator, const Str filename, Image* out_image);

void
deinit_image(const mem::Allocator allocator, Image image);

Image
black_image();

} // namespace img

} // namespace mksv
