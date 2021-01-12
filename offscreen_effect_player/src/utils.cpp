#include "utils.hpp"

namespace bnb
{
    pixel_bufer convert_bpc8_to_y_plane(const bpc8_image_t& image, uint32_t width, uint32_t height)
    {
        pixel_bufer y_pixels(width * height);

        uint8_t* data = image.get_data();

        auto fmt = image.get_pixel_format();
        auto channels = image.bytes_per_pixel(fmt);
        auto offsets = image.rgb_offsets(fmt);

        auto rOff = std::get<0>(offsets);
        auto gOff = std::get<1>(offsets);
        auto bOff = std::get<2>(offsets);

        //TODO swizzle coeffs with rgb offsets for much faster conversion
        const auto rgb2y = [&](uint8_t const* rgb) { return 0.299f * rgb[rOff] + 0.587f * rgb[gOff] + 0.114f * rgb[bOff]; };

        uint8_t const* const p_end = data + width * height * channels;
        uint8_t* y = y_pixels.data();
        for (uint8_t const* p = data; p != p_end; p += channels, ++y) {
            *y = uint8_t(std::min(255.f, std::round(rgb2y(p))));
        }

        return y_pixels;
    }

    pixel_bufer bnb::convert_bpc8_to_u_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv)
    {
        pixel_bufer u_pixels((width / 2) * (height / 2));

        uint8_t* data = image.get_data();

        auto fmt = image.get_pixel_format();
        auto channels = image.bytes_per_pixel(fmt);
        auto offsets = image.rgb_offsets(fmt);

        auto rOff = std::get<0>(offsets);
        auto gOff = std::get<1>(offsets);
        auto bOff = std::get<2>(offsets);

        //TODO swizzle coeffs with rgb offsets for much faster conversion
        auto rgb2u = [&](uint8_t const* rgb) { return -0.169f * rgb[rOff] - 0.331f * rgb[gOff] + 0.500f * rgb[bOff]; };
        auto rgb2u0 = [&](uint8_t const* rgb) { return -0.147f * rgb[rOff] - 0.289f * rgb[gOff] + 0.436f * rgb[bOff]; };

        uint8_t* u_ptr = u_pixels.data();
        for (uint32_t j = 0, h_end = height & ~1; j < h_end; j += 2) {
            for (uint32_t i = 0, w_end = width & ~1; i < w_end; i += 2, ++u_ptr) {
                float u = 0.f;

                for (auto [jOff, iOff] : {std::array<int, 2>{0, 0}, {0, 1}, {1, 0}, {1, 1}}) {
                    auto c = data + ((j + jOff) * width + (i + iOff)) * channels;
                    if (legacy_yuv) {
                        u += rgb2u(c);
                    } else {
                        u += rgb2u0(c);
                    }
                }

                auto& u_out = u_ptr[0];

                u_out = uint8_t(std::max(0.f, std::min(255.f, std::round(128 + u / 4))));
            }
        }

        return u_pixels;
    }

    pixel_bufer bnb::convert_bpc8_to_v_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv)
    {
        pixel_bufer v_pixels((width / 2) * (height / 2));

        uint8_t* data = image.get_data();

        auto fmt = image.get_pixel_format();
        auto channels = image.bytes_per_pixel(fmt);
        auto offsets = image.rgb_offsets(fmt);

        auto rOff = std::get<0>(offsets);
        auto gOff = std::get<1>(offsets);
        auto bOff = std::get<2>(offsets);

        //TODO swizzle coeffs with rgb offsets for much faster conversion
        auto rgb2v = [&](uint8_t const* rgb) { return 0.500f * rgb[rOff] - 0.419f * rgb[gOff] - 0.081f * rgb[bOff]; };
        auto rgb2v0 = [&](uint8_t const* rgb) { return 0.615f * rgb[rOff] - 0.515f * rgb[gOff] - 0.100f * rgb[bOff]; };

        uint8_t* v_ptr = v_pixels.data();
        for (uint32_t j = 0, h_end = height & ~1; j < h_end; j += 2) {
            for (uint32_t i = 0, w_end = width & ~1; i < w_end; i += 2, ++v_ptr) {
                float u = 0.f, v = 0.f;

                for (auto [jOff, iOff] : {std::array<int, 2>{0, 0}, {0, 1}, {1, 0}, {1, 1}}) {
                    auto c = data + ((j + jOff) * width + (i + iOff)) * channels;
                    if (legacy_yuv) {
                        v += rgb2v(c);
                    } else {
                        v += rgb2v0(c);
                    }
                }

                auto& v_out = v_ptr[0];

                v_out = uint8_t(std::max(0.f, std::min(255.f, std::round(128 + v / 4))));
            }
        }

        return v_pixels;
    }

    pixel_bufer bnb::convert_bpc8_to_uv_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv)
    {
        pixel_bufer uv_pixels((width / 2) * (height / 2) * 2);

        uint8_t* data = image.get_data();

        auto fmt = image.get_pixel_format();
        auto channels = image.bytes_per_pixel(fmt);
        auto offsets = image.rgb_offsets(fmt);

        auto rOff = std::get<0>(offsets);
        auto gOff = std::get<1>(offsets);
        auto bOff = std::get<2>(offsets);

        //TODO swizzle coeffs with rgb offsets for much faster conversion
        auto rgb2u = [&](uint8_t const* rgb) { return -0.169f * rgb[rOff] - 0.331f * rgb[gOff] + 0.500f * rgb[bOff]; };
        auto rgb2u0 = [&](uint8_t const* rgb) { return -0.147f * rgb[rOff] - 0.289f * rgb[gOff] + 0.436f * rgb[bOff]; };
        auto rgb2v = [&](uint8_t const* rgb) { return 0.500f * rgb[rOff] - 0.419f * rgb[gOff] - 0.081f * rgb[bOff]; };
        auto rgb2v0 = [&](uint8_t const* rgb) { return 0.615f * rgb[rOff] - 0.515f * rgb[gOff] - 0.100f * rgb[bOff]; };

        uint8_t* uv = uv_pixels.data();
        for (uint32_t j = 0, h_end = height & ~1; j < h_end; j += 2) {
            for (uint32_t i = 0, w_end = width & ~1; i < w_end; i += 2, uv += 2) {
                float u = 0.f, v = 0.f;

                for (auto [jOff, iOff] : {std::array<int, 2>{0, 0}, {0, 1}, {1, 0}, {1, 1}}) {
                    auto c = data + ((j + jOff) * width + (i + iOff)) * channels;
                    if (legacy_yuv) {
                        u += rgb2u(c);
                        v += rgb2v(c);
                    } else {
                        u += rgb2u0(c);
                        v += rgb2v0(c);
                    }
                }

                auto& u_out = uv[0];
                auto& v_out = uv[1];

                u_out = uint8_t(std::max(0.f, std::min(255.f, std::round(128 + u / 4))));
                v_out = uint8_t(std::max(0.f, std::min(255.f, std::round(128 + v / 4))));
            }
        }

        return uv_pixels;
    }
} // bnb