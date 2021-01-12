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

#if BNB_APPLE
    #include <boost/filesystem.hpp>
using boost::filesystem::path;
#else
    #include <filesystem>
using std::filesystem::path;
#endif

class BanubaSdkManager
{
public:
    BanubaSdkManager(
        const std::string& window_title,
        const std::vector<std::string>& path_to_resources,
        const std::string& client_token
        );

    BanubaSdkManager(
        const std::string& client_token,
        int32_t width,
        int32_t height,
        bool manual_audio
        );

    BanubaSdkManager(
        const std::string& window_title,
        const std::vector<std::string>& path_to_resources,
        const std::string& client_token,
        int32_t width,
        int32_t height,
        bool manual_audio
        );

    ~BanubaSdkManager();

    void run_main_loop()
    {
        m_window.run_main_loop();
    }

    void load_effect(const std::string& effectPath, bool synchronous);
    void unload_effect(bool synchronous);

    void process_image(const path& path);
    bnb::data_t sync_process_frame(std::shared_ptr<bnb::full_image_t> image);
    void async_process_frame(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback);

    void process_camera(int camera_id = 0);

    void pause();
    void resume();

    void enable_audio(bool enable);

    void call_js_method(const std::string& method, const std::string& param);

private:
    void start_render_thread();

    GlfwWindow m_window;
    bnb::utility m_utility;
    std::shared_ptr<bnb::interfaces::effect_player> m_effect_player;
    std::shared_ptr<bnb::camera_base> m_camera_ptr;
    std::unique_ptr<RenderThread> m_render_thread;
    bool m_window_is_shown;

    std::pair<int, int> last_frame_size{0, 0};
};