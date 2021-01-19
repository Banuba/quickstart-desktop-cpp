#pragma once

#include "../interfaces/offscreen_effect_player.hpp"

#include <bnb/types/base_types.hpp>
#include <bnb/types/full_image.hpp>

namespace bnb
{
    color_plane convert_bpc8_to_y_plane(const bpc8_image_t& image, uint32_t width, uint32_t height);
    // color_plane convert_bpc8_to_u_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
    // color_plane convert_bpc8_to_v_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
    color_plane convert_bpc8_to_uv_plane(const bpc8_image_t& image, uint32_t width, uint32_t height, bool legacy_yuv);
} // bnb
