#pragma once

#include <GlfwWindow.hpp>
#include <RenderThread.hpp>

#include <string>
#include <vector>
#include <memory>

#include <bnb/utils/interfaces/all.hpp>
#include <bnb/effect_player/interfaces/all.hpp>
#include <bnb/effect_player/utility.hpp>
#include <bnb/spal/camera/base.hpp>
#include <bnb/utils/defs.hpp>
#include <filesystem>

class BanubaSdkManager
{
public:
    static std::string sdk_resources_path();

    BanubaSdkManager(
        const std::string& window_title,
        const std::vector<std::string>& path_to_resources,
        const std::string& client_token);

    ~BanubaSdkManager();

    void run_main_loop()
    {
        m_window.run_main_loop();
    }

    void load_effect(const std::string& effectPath, bool synchronous);

    bnb::data_t process_image(const std::filesystem::path& path);

    void process_camera(int camera_id = 0);

private:
    void start_render_thread();

    GlfwWindow m_window;
    bnb::utility m_utility;
    std::shared_ptr<bnb::interfaces::effect_player> m_effect_player;
    std::shared_ptr<bnb::camera_base> m_camera_ptr;
    std::unique_ptr<RenderThread> m_render_thread;
    bool m_window_is_shown;
};