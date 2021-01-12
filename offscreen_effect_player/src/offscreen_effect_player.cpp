#include "../include/offscreen_effect_player.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "utils.hpp"

namespace bnb
{
    offscreen_effect_player::offscreen_effect_player(const std::string& client_token,
                                    int32_t width, int32_t height, bool manual_audio)
    {
        _manager = std::make_unique<BanubaSdkManager>(client_token, width, height, manual_audio);
    }

    void offscreen_effect_player::async_process_image(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback)
    {
        _manager->async_process_frame(image, callback);
    }

    void offscreen_effect_player::async_process_image_I420(I420_planes yuv, const image_format& image_f, std::function<void(I420_planes planes)> callback)
    {
        pixel_bufer uv(yuv.u.size() * 2);
        for (int32_t i = 0; i < yuv.u.size(); ++i) {
            uv[i * 2] = yuv.u[i];
            uv[i * 2 + 1] = yuv.v[i];
        }

        auto y_plane = color_plane_vector(std::move(yuv.y));
        auto uv_plane = color_plane_vector(std::move(uv));

        bnb::yuv_image_t yuv_img(std::move(y_plane), std::move(uv_plane), image_f);
        std::shared_ptr<bnb::full_image_t> image = std::make_shared<full_image_t>(full_image_t(std::move(yuv_img)));

        auto convert_callback = [image_f, callback](bnb::data_t data) {
            bnb::bpc8_image_t bpc8(
                bnb::color_plane_weak(data.data.get()),
                bnb::bpc8_image_t::pixel_format_t::rgba,
                image_f);

            auto y_plane = convert_bpc8_to_y_plane(bpc8, image_f.width, image_f.height);
            auto u_plane = convert_bpc8_to_u_plane(bpc8, image_f.width, image_f.height, true);
            auto v_plane = convert_bpc8_to_v_plane(bpc8, image_f.width, image_f.height, true);

            callback(I420_planes{ std::move(y_plane), std::move(u_plane), std::move(v_plane) }); 
        };

        async_process_image(image, convert_callback);
    }

    void offscreen_effect_player::async_process_image_I420(pixel_bufer yuv, const image_format& image_f, std::function<void(pixel_bufer data)> callback)
    {
        cv::Mat input(image_f.height * 3 / 2, image_f.width, CV_8UC1, yuv.data());
        cv::cvtColor(input, input, cv::COLOR_YUV2RGBA_I420);
        std::vector<uint8_t> data(static_cast<const uint8_t*>(input.data), static_cast<const uint8_t*>(input.data) + image_f.width * image_f.height * 4);
        bnb::bpc8_image_t rgba_bpc8(bnb::color_plane_vector(data), bnb::interfaces::pixel_format::rgba, image_f);

        async_process_image_bpc8(std::move(rgba_bpc8), image_f, callback, encoding_format::i420);
    }

    void offscreen_effect_player::async_process_image_NV12(NV12_planes nv12, const image_format& image_f, std::function<void(NV12_planes planes)> callback)
    {
        auto y_plane = color_plane_vector(std::move(nv12.y));
        auto uv_plane = color_plane_vector(std::move(nv12.uv));

        bnb::yuv_image_t yuv_img(std::move(y_plane), std::move(uv_plane), image_f);
        std::shared_ptr<bnb::full_image_t> image = std::make_shared<full_image_t>(full_image_t(std::move(yuv_img)));
        
        auto convert_callback = [image_f, callback](bnb::data_t data) {
            bnb::bpc8_image_t bpc8(
                bnb::color_plane_weak(data.data.get()),
                bnb::bpc8_image_t::pixel_format_t::rgba,
                image_f);

            auto y_plane = convert_bpc8_to_y_plane(bpc8, image_f.width, image_f.height);
            auto uv_plane = convert_bpc8_to_uv_plane(bpc8, image_f.width, image_f.height, true);

            callback(NV12_planes{ std::move(y_plane), std::move(uv_plane) }); 
        };

        async_process_image(image, convert_callback);
    }

    // void offscreen_effect_player::async_process_image_NV12(pixel_bufer nv12, const image_format& image_f, std::function<void(pixel_bufer data)> callback)
    // {
    //     cv::Mat input(image_f.height * 3 / 2, image_f.width, CV_8UC1, nv12.data());
    //     cv::cvtColor(input, input, cv::COLOR_YUV2RGBA_NV12);
    //     std::vector<uint8_t> data(static_cast<const uint8_t*>(input.data), static_cast<const uint8_t*>(input.data) + image_f.width * image_f.height * 4);
    //     bnb::bpc8_image_t rgba_bpc8(bnb::color_plane_vector(data), bnb::interfaces::pixel_format::rgba, image_f);

    //     async_process_image_bpc8(std::move(rgba_bpc8), image_f, callback, encoding_format::nv12);
    // }

    void offscreen_effect_player::async_process_image_RGBA(pixel_bufer rgba, const image_format& image_f, std::function<void(pixel_bufer data)> callback)
    {
        bnb::bpc8_image_t rgba_bpc8(
            bnb::color_plane_weak(rgba.data()),
            bnb::bpc8_image_t::pixel_format_t::rgba,
            image_f);

        async_process_image_bpc8(std::move(rgba_bpc8), image_f, callback, encoding_format::rgba);
    }

    void offscreen_effect_player::load_effect(const std::string& effect_name, bool synchronous)
    {
        _manager->load_effect(effect_name, synchronous);
    }

    void offscreen_effect_player::unload_effect(bool synchronous)
    {
        _manager->unload_effect(synchronous);
    }

    void offscreen_effect_player::pause()
    {
        _manager->pause();
    }

    void offscreen_effect_player::resume()
    {
        _manager->resume();
    }

    void offscreen_effect_player::enable_audio(bool enable)
    {
        _manager->enable_audio(enable);
    }

    void offscreen_effect_player::call_js_method(const std::string& method, const std::string& param)
    {
        _manager->call_js_method(method, param);
    }

    void offscreen_effect_player::async_process_image_bpc8(bpc8_image_t&& image_bpc8, const image_format& image_f, std::function<void(pixel_bufer data)> callback, encoding_format ouput_format)
    {
        std::shared_ptr<bnb::full_image_t> image = std::make_shared<full_image_t>(full_image_t(std::move(image_bpc8)));
        
        auto converter_callback = [image_f, callback, ouput_format](bnb::data_t data) {
            const auto result_size = size_t(image_f.width * image_f.height * 4);

            if (ouput_format == encoding_format::rgba) {
                pixel_bufer pixel_bufer(data.data.get(), data.data.get() + result_size);
                callback(pixel_bufer);
                return;
            }

            cv::Mat in(image_f.height, image_f.width, CV_8UC(4), data.data.get());

            switch (ouput_format)
            {
                case encoding_format::i420:
                    cv::cvtColor(in, in, cv::COLOR_RGBA2YUV_I420);
                    break;
                case encoding_format::nv12:
                    cv::cvtColor(in, in, cv::COLOR_RGBA2YUV_YV12);
                    break;
            }
            pixel_bufer pixel_bufer(result_size);

            auto rot = static_cast<uint32_t>(image_f.orientation) * 90;
            cv::Mat out(image_f.height, image_f.width, CV_8UC(4), pixel_bufer.data());
            cv::rotate(in, out, rot);
            callback(pixel_bufer);
        };
        
        _manager->async_process_frame(image, converter_callback);
    }

} // bnb