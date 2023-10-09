#include "image.hpp"

#include "fmt.hpp"
#include "io.hpp"
#include "utils.hpp"

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

#pragma pack(pop)

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

bool
load_tga(const mem::Allocator allocator, const Str filename, Image* out_image) {
    mem::Slice<u8> image_data = {};
    if (!io::read_file(allocator, filename, &image_data)) return false;

    const TGAHeader* header = (TGAHeader*)image_data.ptr;
    const bool has_color_map = header->color_map_type;

    const TGAImageType type = (TGAImageType)(header->image_type & 0b11);
    const TGACompressionType compression = (TGACompressionType)(header->image_type & 0b1000);

    (void)type;
    (void)compression;
    (void)has_color_map;
    (void)out_image;

    return false;
}

enum struct BMPCompressionType {
    Rgb = 0,
    Rle8 = 1,
    Rle4 = 2,
    Bitfields = 3,
    Jpeg = 4,
    Png = 5,
    AlphaBitfields = 6,
};

#pragma pack(push)
#pragma pack(1)

struct BMPHeader {
    u8 ident[2];
    u32 filesize;
    u32 reserved;
    u32 image_offset;
};

struct DIBHeader {
    u32 header_size;
    i32 width;
    i32 height;
    u16 n_color_planes;
    u16 bpp;
    BMPCompressionType compression;
    u32 image_size;
    i32 x_pixels_per_meter;
    i32 y_pixels_per_meter;
    u32 n_colors;
    u32 n_important_colors;
    u32 red_mask;
    u32 green_mask;
    u32 blue_mask;
    u32 alpha_mask;
    u32 color_space;
};

#pragma pack(pop)

static bool
is_bmp_compression_supported(const BMPCompressionType type) {
    return type == BMPCompressionType::Bitfields;
}

[[nodiscard]] bool
load_bmp(const mem::Allocator allocator, const Str filename, Image* out_image) {
    mem::Slice<u8> image_data = {};
    if (!io::read_file(allocator, filename, &image_data)) return false;

    const BMPHeader* bmp_header = (BMPHeader*)image_data.ptr;
    const DIBHeader* header = (DIBHeader*)(image_data.ptr + sizeof(BMPHeader));

    const bool is_srgb = header->color_space == 'sRGB' || header->color_space == 'Win ';

    if (!mem::equal(Str{ "BM" }, Str{ (u8*)bmp_header->ident, 2 }) || !is_srgb) {
        fmt::print_stderr("Unsupported BMP version\n");
        return false;
    }

    if (header->bpp != 32) {
        fmt::print_stderr("Unsupported bpp format\n");
        return false;
    }

    if (header->n_color_planes != 1) {
        fmt::print_stderr("Number of color planes is not 1\n");
        return false;
    }

    if (!is_bmp_compression_supported(header->compression)) {
        fmt::print_stderr("Unsupported compression\n");
        return false;
    }

    if (header->n_colors != 0) {
        fmt::print_stderr("Color palettes unsupported\n");
        return false;
    }

    const bool flipped = header->height < 0;
    const u32 stride = mem::align_up<u32>(header->bpp * (u32)header->width / 32, 4);
    u32* pixel_row = (u32*)(image_data.ptr + bmp_header->image_offset);

    Image img = {
        .width = (u32)header->width,
        .height = flipped ? (u32)-header->height : (u32)header->height,
        .bpp = header->bpp,
    };
    if (!allocator.alloc(stride * img.height, &img.pixels)) {
        fmt::print_stderr("Failed to allocate memory\n");
        return false;
    }

    if (flipped) pixel_row += stride * (img.height - 1);
    const u32 r_shift = clz(header->red_mask);
    const u32 g_shift = clz(header->green_mask);
    const u32 b_shift = clz(header->blue_mask);
    const u32 a_shift = clz(header->alpha_mask);

    for (u64 j = 0; j < img.height; ++j) {
        for (u64 i = 0; i < img.width; ++i) {
            const u32 pixel = pixel_row[i];
            const u32 r = (pixel & header->red_mask) >> r_shift;
            const u32 g = (pixel & header->green_mask) >> g_shift;
            const u32 b = (pixel & header->blue_mask) >> b_shift;
            const u32 a = (pixel & header->alpha_mask) >> a_shift;
            const u32 out = (a << 24) | (b << 16) | (g << 8) | r;
            img.pixels.ptr[j * img.width + i] = out;
        }
        if (flipped)
            pixel_row -= stride;
        else
            pixel_row += stride;
    }

    *out_image = img;

    return true;
}

} // namespace img
} // namespace mksv
