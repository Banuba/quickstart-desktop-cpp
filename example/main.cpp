
#include "BanubaClientToken.hpp"

#include "../offscreen_effect_player/include/offscreen_effect_player.hpp"
#include "../offscreen_effect_player/interfaces/offscreen_effect_player.hpp"

#include "renderer/render_thread.hpp"

using namespace std::chrono;

int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    std::unique_ptr<bnb::interfaces::offscreen_effect_player> oep =
            std::make_unique<bnb::offscreen_effect_player>(BNB_CLIENT_TOKEN, width, height, false);
    oep->load_effect("effects/virtual_bg", true);

    std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
    std::shared_ptr<render::RenderThread> render_t =
            std::make_shared<render::RenderThread>(window->get_window(), width, height);

    auto ef_cb = [&oep, render_t](bnb::full_image_t image) {
        auto call_back = [render_t](bnb::data_t data) {
            render_t->update_data(std::move(data));
            render_t->schedule([render_t]() mutable {
                render_t->update_context();
            });
        };
        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));
        oep->async_process_image(image_ptr, call_back);
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    window->show(width, height);
    window->run_main_loop();

    return 0;
}