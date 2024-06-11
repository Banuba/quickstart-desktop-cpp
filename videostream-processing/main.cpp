#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>

#include <bnb/effect_player/utility.hpp>
#include <bnb/effect_player/interfaces/types.hpp>

#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input.hpp>
#include <bnb/player_api/interfaces/output/window_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>
#include <bnb/player_api/interfaces/render_target/metal_render_target.hpp>

#include <stb_gif.hpp>

#include <thread>
#include <filesystem>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace bnb::interfaces;

namespace
{
    render_backend_type render_backend = render_backend_type::opengl;
}


// Custom input for gif file
class gif_stream_input : public bnb::player_api::interfaces::input
{
public:
    gif_stream_input(const std::string& path)
    : m_gif(path)
    {
        auto config = bnb::interfaces::processor_configuration::create();
        m_frame_processor = bnb::interfaces::frame_processor::create_video_processor(config);
        m_gif.rewind();
        
        m_thread = std::thread([this](){;
            bnb::image_format format;
            
            format.width = m_gif.width();
            format.height = m_gif.height();
            format.orientation = bnb::camera_orientation::deg_0;
            
            while (!m_quit) {
                if(!m_gif.jump_to_next_frame()){ // End of file
                    m_gif.rewind();
                    std::this_thread::yield();
                    continue;
                }
                auto frame = m_gif.current_frame();
        
                bnb::bpc8_image_t::pixel_format_t pixel_format{bnb::bpc8_image_t::pixel_format_t::rgba};
                auto color_plane = bnb::color_plane(const_cast<bnb::color_plane_data_t*>(get_pixel_data(frame)), [frame](bnb::color_plane_data_t*) {
                    // Hold the copy of the frame here, unil deleter called
                });
                
                bnb::bpc8_image_t rgba_image(color_plane, pixel_format, format);
                bnb::full_image_t image(std::move(rgba_image));
                auto fd = bnb::interfaces::frame_data::create();
                fd->add_full_img(std::move(image));
                m_frame_processor->push(fd);
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_timestamp_us = frame.timepoint();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(frame.delay()));
            }
        });

    }
    ~gif_stream_input()
    {
        m_thread.join();
    }
    
    bnb::player_api::frame_processor_sptr get_frame_processor() const noexcept override
    {
        return m_frame_processor;
    }
    
    uint64_t get_frame_time_us() const noexcept override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_timestamp_us;
    }
    
    void on_quit()
    {
        m_quit = true;
    }
    
private:
    uint8_t* get_pixel_data(const stbi::gif::frame& frame)
    {
        if (!frame) {
            return {};
        }

        return static_cast<uint8_t*>(const_cast<void*>(frame.data()));
    }
    
    uint64_t m_timestamp_us{0};
    bnb::player_api::frame_processor_sptr m_frame_processor{nullptr};
    std::atomic<bool> m_quit{false};
    std::thread m_thread;
    stbi::gif m_gif;
    mutable std::mutex m_mutex;
};


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
    // Create custom input for gif
    auto input = std::make_shared<gif_stream_input>(std::filesystem::path(BNB_RESOURCES_FOLDER) / "face600x600.gif");
    // on-screen output
    auto window_output = bnb::player_api::window_output::create(renderer->get_native_surface());
    
    player->use(input).use(window_output);
    player->load_async("effects/TrollGrandma");
    
    // Setup callbacks for glfw window
    // resize and on-quit events
    renderer->get_window()->set_callbacks([window_output](uint32_t w, uint32_t h){
        window_output->set_frame_layout(0, 0, w, h);
    }, [input](){
        input->on_quit();
    });
    
    // Run main loop
    renderer->get_window()->show_window_and_run_events_loop();
    return 0;
}
