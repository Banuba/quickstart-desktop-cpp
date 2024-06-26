#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>

#include <bnb/effect_player/utility.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/live_input.hpp>
#include <bnb/player_api/interfaces/input/photo_input.hpp>
#include <bnb/player_api/interfaces/output/opengl_frame_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>
#include <bnb/player_api/interfaces/render_target/metal_render_target.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <filesystem>

using namespace bnb::interfaces;

namespace
{
    render_backend_type render_backend = render_backend_type::opengl;
}

int main()
{
    #if defined(__APPLE__)
    const auto result_path = (std::filesystem::temp_directory_path() / "bnb_result.jpg").string();
    #else
    const auto result_path = (std::filesystem::current_path() / "bnb_result.jpg").string();
    #endif
    
    // Initialize BanubaSDK with token and paths to resources
    bnb::utility utility({bnb::sdk_resources_path(), BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    // Create render delegate based on GLFW
    auto renderer = std::make_shared<GLFWRenderer>(render_backend);
    // Create render target
    auto render_target = bnb::player_api::opengl_render_target::create();
    // Create player
    auto player = bnb::player_api::player::create(30, render_target, renderer);
    // Create photo input
    auto input = bnb::player_api::photo_input::create();
    // Create frame output with callback t
    // that is called when frames are received and saves it to a file.
    auto frame_output = bnb::player_api::opengl_frame_output::create([player, result_path](const bnb::full_image_t& pb) {
        stbi_write_png(
            (result_path + ".png").c_str(),
            pb.get_width(),
            pb.get_height(),
            pb.get_bytes_per_pixel(),
            pb.get_base_ptr(),
            pb.get_bytes_per_row()
        );
        std::printf("Processing result was written to `%s`. \n", result_path.c_str());
    }, bnb::pixel_buffer_format::bpc8_rgba);
    // Sync effect loading
    player->load("effects/TrollGrandma");
    player->use(input).use(frame_output);
    // Switch to manual render mode
    player->set_render_mode(bnb::player_api::player::render_mode::manual);
    // Load image into input
    input->load((std::filesystem::path(BNB_RESOURCES_FOLDER) / "face720x1280.jpg").string());
    // Render once
    player->render();
}
