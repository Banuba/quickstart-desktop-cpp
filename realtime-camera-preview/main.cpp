#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>

#include <bnb/spal/camera/base.hpp>
#include <bnb/effect_player/utility.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/live_input.hpp>
#include <bnb/player_api/interfaces/output/window_output.hpp>
#include <bnb/player_api/interfaces/output/opengl_frame_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>

using namespace bnb::interfaces;

int main()
{
    bnb::utility utility({bnb::sdk_resources_path(), BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    auto renderer = std::make_shared<glfw_renderer>();
    auto render_target = bnb::player_api::opengl_render_target::create();
    // Create player
    auto player = bnb::player_api::player::create(30, render_target, renderer);
    // Create live input, for realtime camera
    auto input = bnb::player_api::live_input::create();
    // On-screen output
    auto window_output = bnb::player_api::window_output::create();
    
    player->use(input).use(window_output);
    player->load_async("effects/TrollGrandma");

    // Create camera device input with callback
    // that is called when new frames are received.
    auto camera = bnb::create_camera_device([input](bnb::full_image_t image) {
        input->push(image);
    }, 0);
    
    // Setup callbacks for glfw window
    renderer->get_window()->set_glfw_events_callback([window_output](const glfw_event& e) {
        if (e.type == glfw_event_t::framebuffer_resize) {
            window_output->set_frame_layout(0, 0, e.size_width, e.size_height);
        }
    });
    
    // Run main loop
    renderer->get_window()->show_window_and_run_events_loop();
    return 0;
}
