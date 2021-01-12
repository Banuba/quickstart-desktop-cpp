#pragma once

#include "../interfaces/offscreen_effect_player.hpp"

#include <bnb/types/base_types.hpp>
#include <bnb/types/full_image.hpp>

namespace bnb
{
    pixel_bufer convert_bpc8_to_y_plane(const bpc8_image_t& image, uint32_t width, uint32_t height);
    pixel_bufer convert_bpc8_to_u_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
    pixel_bufer convert_bpc8_to_v_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
    pixel_bufer convert_bpc8_to_uv_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
} // bnb
