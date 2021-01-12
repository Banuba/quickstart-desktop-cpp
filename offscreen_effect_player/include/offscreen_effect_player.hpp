#pragma once

#include "../interfaces/offscreen_effect_player.hpp"

#include <BanubaSdkManager.hpp>

namespace bnb
{
    class offscreen_effect_player: public interfaces::offscreen_effect_player
    {
    public:
        offscreen_effect_player(const std::string& client_token, int32_t width, int32_t height, bool manual_audio);

        void async_process_image(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback) override;

        void async_process_image_I420(I420_planes yuv, const image_format& image_f, std::function<void(I420_planes planes)> callback) override;
        void async_process_image_I420(pixel_bufer yuv, const image_format& image_f, std::function<void(pixel_bufer data)> callback) override;

        void async_process_image_NV12(NV12_planes nv12, const image_format& image_f, std::function<void(NV12_planes planes)> callback) override;
        // void async_process_image_NV12(pixel_bufer nv12, const image_format& image_f, std::function<void(pixel_bufer data)> callback) override;

        void async_process_image_RGBA(pixel_bufer rgba, const image_format& image_f, std::function<void(pixel_bufer data)> callback) override;

        void load_effect(const std::string& effect_name, bool synchronous) override;
        void unload_effect(bool synchronous) override;

        void pause() override;
        void resume() override;

        void enable_audio(bool enable) override;

        void call_js_method(const std::string& method, const std::string& param) override;

    private:
        void async_process_image_bpc8(bpc8_image_t&& image_bpc8, const image_format& image_f, std::function<void(pixel_bufer data)> callback, encoding_format ouput_format);

        std::unique_ptr<BanubaSdkManager> _manager;
    };
} // bnb
