#include <BanubaSdkManager.hpp>

#include "BanubaClientToken.hpp"

#include "../offscreen_effect_player/include/offscreen_effect_player.hpp"

// #include "renderer/camera_renderer.hpp"
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

    auto ef_cb = [](bnb::full_image_t image) {

        std::cout << image.get_format().width << std::endl;
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    // bnb::camera_renderer render;

    std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
    window->show(width, height);
    window->run_main_loop();

    while (true) {}

    return 0;
}