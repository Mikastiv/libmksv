#include "image.hpp"

#include "bit.hpp"
#include "error.hpp"
#include "fmt.hpp"
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

    const mem::Slice<u8> image_id = { image_data.ptr + sizeof(TGAHeader), header->id_len };
    const mem::Slice<u8> color_map = { image_id.ptr + image_id.len, header->color_map_len };

    const mem::Slice<u8> pixels = { color_map.ptr + color_map.len,
                                    (u64)header->width * (u64)header->height *
                                        (u64)header->pixel_depth };

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
    return type == BMPCompressionType::Bitfields || type == BMPCompressionType::Rgb;
}

[[nodiscard]] bool
load_bmp(const mem::Allocator allocator, const Str filename, Image* out_image) {
    mem::Slice<u8> image_data = {};
    if (!io::read_file(allocator, filename, &image_data)) return false;

    const BMPHeader* bmp_header = (BMPHeader*)image_data.ptr;
    const DIBHeader* header = (DIBHeader*)(image_data.ptr + sizeof(BMPHeader));

    const bool is_srgb = header->color_space == 'sRGB' || header->color_space == 'Win ';

    if (!mem::equal(Str{ "BM" }, Str{ (u8*)bmp_header->ident, 2 }) || !is_srgb)
        return err::error("Unsupported BMP version\n");

    if (header->bpp != 32 && header->bpp != 24) {
        fmt::print_stderr("Unsupported bpp format\n");
        return false;
    }

    if (header->n_color_planes != 1) return err::error("Number of color planes is not 1\n");
    if (!is_bmp_compression_supported(header->compression))
        return err::error("Unsupported compression\n");
    if (header->n_colors != 0) return err::error("Color palettes unsupported\n");

    const bool flipped = header->height < 0;
    const u32 pixel_width = header->bpp / 8;
    const u32 stride = mem::align_up<u32>((header->bpp * (u32)header->width / 32) * 4, 4);
    u8* pixel_row = image_data.ptr + bmp_header->image_offset;

    Image img = {
        .width = (u32)header->width,
        .height = flipped ? (u32)-header->height : (u32)header->height,
        .bpp = header->bpp,
    };

    if (!allocator.alloc(img.width * 4 * img.height, &img.pixels)) return ALLOC_FAILED;

    if (flipped) pixel_row += stride * (img.height - 1);
    const i32 increment = flipped ? -(i32)stride : (i32)stride;

    switch (header->compression) {
        case BMPCompressionType::Bitfields: {
            const u32 r_shift = bit::clz(header->red_mask);
            const u32 g_shift = bit::clz(header->green_mask);
            const u32 b_shift = bit::clz(header->blue_mask);
            const u32 a_shift = bit::clz(header->alpha_mask);

            for (u64 j = 0; j < img.height; ++j) {
                for (u64 i = 0; i < img.width; ++i) {
                    const u32 pixel = *(u32*)&pixel_row[i * pixel_width];
                    const u32 r = (pixel & header->red_mask) >> r_shift;
                    const u32 g = (pixel & header->green_mask) >> g_shift;
                    const u32 b = (pixel & header->blue_mask) >> b_shift;
                    const u32 a = (pixel & header->alpha_mask) >> a_shift;
                    const u32 out = (a << 24) | (b << 16) | (g << 8) | r;
                    img.pixels.ptr[j * img.width + i] = out;
                }
                pixel_row += increment;
            }
        } break;
        case BMPCompressionType::Rgb: {
            for (u64 j = 0; j < img.height; ++j) {
                for (u64 i = 0; i < img.width; ++i) {
                    const u32 b = pixel_row[i * pixel_width];
                    const u32 g = pixel_row[i * pixel_width + 1];
                    const u32 r = pixel_row[i * pixel_width + 2];
                    const u32 a = 0xFF;
                    const u32 out = (a << 24) | (b << 16) | (g << 8) | r;
                    img.pixels.ptr[j * img.width + i] = out;
                }
                pixel_row += increment;
            }
        } break;
        default: {
            return ERROR("Unreachable\n");
        } break;
    }

    *out_image = img;

    return true;
}

static const u32 black_pixel = 0;

void
deinit_image(const mem::Allocator allocator, Image image) {
    if (image.pixels.ptr != &black_pixel) allocator.free(image.pixels);
}

Image
black_image() {
    return {
        .width = 1,
        .height = 1,
        .bpp = 32,
        .pixels = {(u32*)&black_pixel, 1},
    };
}

} // namespace img
} // namespace mksv
