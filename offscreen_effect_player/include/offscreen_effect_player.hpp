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

        void load_effect(const std::string& effect_name, bool synchronous) override;
        void unload_effect(bool synchronous) override;

        void pause() override;
        void resume() override;

        void enable_audio(bool enable) override;

        void call_js_method(const std::string& method, const std::string& param) override;

    private:
        std::unique_ptr<BanubaSdkManager> _manager;
    };
} // bnb
