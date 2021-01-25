#include "../include/offscreen_effect_player.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

} // bnb