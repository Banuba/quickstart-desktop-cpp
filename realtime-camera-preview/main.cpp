#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>

#include <bnb/utils/defs.hpp>
#include <bnb/spal/camera/base.hpp>
#include <bnb/effect_player/utility.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/live_input.hpp>
#include <bnb/player_api/interfaces/output/window_output.hpp>
#include <bnb/player_api/interfaces/output/opengl_frame_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>
#include <bnb/player_api/interfaces/render_target/metal_render_target.hpp>

using namespace bnb::interfaces;

namespace
{
    render_backend_type render_backend = BNB_APPLE ? render_backend_type::metal : render_backend_type::opengl;
}

int main()
{
    // Initialize BanubaSDK with token and paths to resources
    bnb::utility utility({bnb::sdk_resources_path(), BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    // Create render delegate based on GLFW
    auto renderer = std::make_shared<GLFWRenderer>(render_backend);
    // Create render target
    bnb::player_api::render_target_sptr render_target;
    if (render_backend == render_backend_type::opengl) {
        render_target = bnb::player_api::opengl_render_target::create();
    } else if (render_backend == render_backend_type::metal) {
        render_target = bnb::player_api::metal_render_target::create();
    }
    // Create player
    auto player = bnb::player_api::player::create(30, render_target, renderer);
    // Create live input, for realtime camera
    auto input = bnb::player_api::live_input::create();
    // On-screen output
    auto window_output = bnb::player_api::window_output::create(renderer->get_native_surface());
    
    player->use(input).use(window_output);
    player->load_async("effects/TrollGrandma");

    // Create camera device input with callback
    // that is called when new frames are received.
    auto camera = bnb::create_camera_device([input](bnb::full_image_t image) {
        auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
        input->push(image, now_us);
    }, 0);
    
    // Setup callbacks for glfw window
    renderer->get_window()->set_callbacks([window_output](uint32_t w, uint32_t h){
        window_output->set_frame_layout(0, 0, w, h);
    }, nullptr);
    
    // Run main loop
    renderer->get_window()->show_window_and_run_events_loop();
    return 0;
}
