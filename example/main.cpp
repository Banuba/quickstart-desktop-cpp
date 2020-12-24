#include <BanubaSdkManager.hpp>

#include "BanubaClientToken.hpp"

#include <chrono>
#include <iostream>

using namespace std::chrono;

//async
int main()
{
    BanubaSdkManager sdk("Realtime Camera Preview", {BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    sdk.load_effect("effects/virtual_bg", true);

    auto ef_cb = [&sdk](bnb::full_image_t image) {
        auto call_back = [](bnb::data_t data) {
        };
        auto ptr_image = std::make_shared<bnb::full_image_t>(std::move(image));
        sdk.async_process_frame_with_show(ptr_image, call_back);
    };
    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);
    sdk.run_main_loop();
    return 0;
}

//sync
// int main()
// {
//     BanubaSdkManager sdk("Realtime Camera Preview", { BNB_RESOURCES_FOLDER }, BNB_CLIENT_TOKEN);
//     sdk.load_effect("effects/test_BG", true);

//     auto ef_cb = [&sdk](bnb::full_image_t image) {
//         auto ptr_image = std::make_shared<bnb::full_image_t>(std::move(image));
//         sdk.sync_process_frame_with_show(ptr_image);
//     };
//     std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);
//     sdk.run_main_loop();
//     return 0;
// }