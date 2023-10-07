#include "image.hpp"

#include "io.hpp"

namespace mksv {
namespace img {

#pragma pack(push)
#pragma pack(1)

struct TGAHeader {
    u8 id_len;
    u8 color_map_type;
    u8 image_type;
    u16 first_entry_idx;
    u16 color_map_len;
    u8 color_map_entry_size;
    u16 x_origin;
    u16 y_origin;
    u16 width;
    u16 height;
    u8 pixel_depth;
    u8 image_descriptor;
};

enum struct TGAImageType {
    NoData = 0b00,
    ColorMapped = 0b01,
    TrueColor = 0b10,
    GrayScale = 0b11,
};

enum struct TGACompressionType {
    Uncompressed = 0,
    RunLengthEncoding = 1,
};

#pragma pack(pop)

bool
load_tga(const mem::Allocator allocator, const Str filename, Image* out_image) {
    mem::Slice<u8> image_data = {};
    if (!io::read_file(allocator, filename, &image_data)) return false;

    const TGAHeader* header = (TGAHeader*)&image_data.ptr;
    const bool has_color_map = header->color_map_type;

    const TGAImageType type = (TGAImageType)(header->image_type & 0b11);
    const TGACompressionType compression = (TGACompressionType)(header->image_type & 0b1000);

    (void)type;
    (void)compression;
    (void)has_color_map;
    (void)out_image;

    return false;
}

} // namespace img
} // namespace mksv
