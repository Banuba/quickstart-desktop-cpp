#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <bnb/types/base_types.hpp>
#include <bnb/types/full_image.hpp>

namespace bnb
{
    using pixel_bufer = std::vector<color_plane_data_t>;

    struct I420_planes
    {
        pixel_bufer y;
        pixel_bufer u;
        pixel_bufer v;
    };

    struct NV12_planes
    {
        pixel_bufer y;
        pixel_bufer uv;
    };

    enum class encoding_format: uint8_t
    {
        i420,
        nv12,
        rgba
    };

    namespace interfaces {
        class offscreen_effect_player
        {
        public:
            virtual ~offscreen_effect_player() = default;

            //async
            virtual void async_process_image(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback) = 0;

            virtual void async_process_image_I420(I420_planes yuv, const image_format& image_f, std::function<void(I420_planes planes)> callback) = 0;
            virtual void async_process_image_I420(pixel_bufer yuv, const image_format& image_f, std::function<void(pixel_bufer data)> callback) = 0;

            virtual void async_process_image_NV12(NV12_planes nv12, const image_format& image_f, std::function<void(NV12_planes planes)> callback) = 0;
            // virtual void async_process_image_NV12(pixel_bufer nv12, const image_format& image_f, std::function<void(pixel_bufer data)> callback) = 0;

            virtual void async_process_image_RGBA(pixel_bufer rgba, const image_format& image_f, std::function<void(pixel_bufer data)> callback) = 0;

            virtual void load_effect(const std::string& effect_path, bool synchronous) = 0;
            virtual void unload_effect(bool synchronous) = 0;

            virtual void pause() = 0;
            virtual void resume() = 0;

            virtual void enable_audio(bool enable) = 0;

            virtual void call_js_method(const std::string& method, const std::string& param) = 0;
        };
    } // interfaces
} // bnb

    void async_process_frame(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback);