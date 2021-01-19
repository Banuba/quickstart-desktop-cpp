#include <BanubaSdkManager.hpp>

#include "BanubaClientToken.hpp"

#include "../offscreen_effect_player/include/offscreen_effect_player.hpp"
#include "renderer/render_thread.hpp"

#include <iostream>

using namespace std::chrono;

// int main()
// {
//     int32_t width = 1280;
//     int32_t height = 720;
    
//     std::unique_ptr<bnb::offscreen_effect_player> oep = std::make_unique<bnb::offscreen_effect_player>(BNB_CLIENT_TOKEN, width, height, false);

//     oep->load_effect("effects/virtual_bg", true);

//     auto ef_cb = [&oep](bnb::full_image_t image) {
//         auto call_back = [](bnb::data_t data) {
//             std::cout << data.size << std::endl;
//         };
//         auto ptr_image = std::make_shared<bnb::full_image_t>(std::move(image));
//         oep->async_process_image(ptr_image, call_back);
//     };
//     std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

//     return 0;
// }

int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
    std::cout << "START" << std::endl;
    std::shared_ptr<render::RenderThread> render_t = std::make_shared<render::RenderThread>(window->get_window());

    auto ef_cb = [render_t](bnb::full_image_t image) {
        std::cout << "Camera send frame to render" << std::endl;

        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));

        render_t->schedule([render_t, image_ptr]() mutable {
            render_t->update_context(image_ptr);
        });
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    window->show(width, height);
    std::cout << "Running" << std::endl;
    window->run_main_loop();

    return 0;
}