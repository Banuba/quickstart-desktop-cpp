#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>

#include <bnb/effect_player/utility.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/stream_input.hpp>
#include <bnb/player_api/interfaces/output/window_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>

#include <stb_gif.hpp>

#include <thread>
#include <filesystem>
#include <chrono>
#include <atomic>

bnb::data_t get_pixel_data(const stbi::gif::frame& frame) 
{
    if (!frame) {
        return {};
    }

    return bnb::data_t::create_non_owning(static_cast<uint8_t*>(const_cast<void*>(frame.data())), frame.size());
}

int main()
{
    bnb::utility utility({bnb::sdk_resources_path(), BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    auto renderer = std::make_shared<glfw_renderer>();
    auto render_target = bnb::player_api::opengl_render_target::create();
    // Create player
    auto player = bnb::player_api::player::create(30, render_target, renderer);
    // Create stream input for gif 
    auto input = bnb::player_api::stream_input::create();
    // on-screen output
    auto window_output = bnb::player_api::window_output::create();
    
    player->use(input).use(window_output);
    player->load_async("effects/TrollGrandma");
    
    std::atomic<bool> quit{false};
    // Emulates video stream processing with gif file
    auto gif_thread = std::thread([&quit, input](){
        stbi::gif gif(std::filesystem::path(BNB_RESOURCES_FOLDER) / "face600x600.gif");
        gif.rewind();
        
        bnb::image_format format;
        
        format.width = gif.width();
        format.height = gif.height();
        format.orientation = bnb::camera_orientation::deg_0;
        
        while (!quit) {
            if(!gif.jump_to_next_frame()){ // End of file
                gif.rewind();
                std::this_thread::yield();
                continue;
            }
            auto frame = gif.current_frame();

            bnb::bpc8_image_t::pixel_format_t pixel_format{bnb::bpc8_image_t::pixel_format_t::rgba};
            auto color_plane = bnb::color_plane(const_cast<bnb::color_plane_data_t*>(get_pixel_data(frame).data.get()), [frame](bnb::color_plane_data_t*) {
                // Hold the copy of the frame here, unil deleter called
            });
            bnb::bpc8_image_t rgba_image(color_plane, pixel_format, format);
            bnb::full_image_t image(std::move(rgba_image));
            
            input->push(image);
            std::this_thread::sleep_for(std::chrono::milliseconds(frame.delay()));
        }
    });
    
    // Setup callbacks for glfw window
    renderer->get_window()->set_glfw_events_callback([window_output, &quit](const glfw_event& e) {
        if (e.type == glfw_event_t::framebuffer_resize) {
            window_output->set_frame_layout(0, 0, e.size_width, e.size_height);
        }
        if(e.type == glfw_event_t::window_close){
            quit = true;
        }
    });
    
    // Run main loop
    renderer->get_window()->show_window_and_run_events_loop();
    
    gif_thread.join();
    return 0;
}
