#include "BanubaSdkManager.hpp"

#include <bnb/effect_player/utility.hpp>
#include <bnb/spal/camera/base.hpp>

BanubaSdkManager::BanubaSdkManager(
    const std::string& window_title,
    const std::vector<std::string>& paths_to_resources,
    const std::string& client_token)
    : m_utility(paths_to_resources, client_token)
    , m_window(window_title)
    , m_effect_player(bnb::interfaces::effect_player::create(
        bnb::interfaces::effect_player_configuration::create(
          720 /*fx_width*/,
          1280 /*fx_height*/
        )
    ))
    , m_window_is_shown(false)
    , m_config(bnb::interfaces::processor_configuration::create())
{
    glfwSetWindowUserPointer(m_window.get_window(), this);
    // Window size changed
    glfwSetFramebufferSizeCallback(m_window.get_window(), [](GLFWwindow* window, int width, int height) {
        auto sdk = static_cast<BanubaSdkManager*>(glfwGetWindowUserPointer(window));
        sdk->m_render_thread->update_surface_size(width, height);
    });
    // Pause on minimizing
    glfwSetWindowIconifyCallback(m_window.get_window(), [](GLFWwindow* window, int iconify) {
        auto sdk = static_cast<BanubaSdkManager*>(glfwGetWindowUserPointer(window));
        if (iconify == GLFW_TRUE)
            sdk->m_effect_player->playback_pause();
        else
            sdk->m_effect_player->playback_play();
    });
    
#if !BNB_GL_BACKEND
    m_effect_player->effect_manager()->set_render_surface(m_window.get_surface_data());
#endif
    
    start_render_thread();
}

BanubaSdkManager::~BanubaSdkManager()
{
    m_camera_ptr = nullptr; // this will free camera
}

void BanubaSdkManager::load_effect(const std::string& effectPath, bool synchronous)
{
    if (synchronous) {
        m_render_thread->schedule([this, effectPath]() {
            m_effect_player->effect_manager()->load(effectPath);
        });
    } else {
        m_effect_player->effect_manager()->load_async(effectPath);
    }
}

std::optional<bnb::interfaces::pixel_buffer> BanubaSdkManager::process_image(const std::filesystem::path& path)
{
    if(m_image_processor == nullptr){
        m_image_processor = bnb::interfaces::frame_processor::create_photo_processor(m_config);
    }
    m_effect_player->set_frame_processor(m_image_processor);
    
    return m_render_thread->schedule([this, path]() -> std::optional<bnb::interfaces::pixel_buffer> {

                    auto name = path.filename().string();
                    auto img = bnb::full_image_t::load(path.string());
                    auto fmt = img.get_format();
        
                    auto fd = bnb::interfaces::frame_data::create();
                    fd->add_full_img(std::move(img));
                    fd->add_frame_number(0);
                    fd->add_timestamp_us(0);

                    auto processor = m_effect_player->frame_processor();
                    processor->push(fd);
                    auto [status, processed_fd] = processor->pop();
                    if (!processed_fd) {
                        return std::nullopt;
                    }

                    auto draw_result = m_effect_player->draw_with_external_frame_data(processed_fd);
                    if (draw_result < 0) {
                        return std::nullopt;
                    }
        
                    return m_effect_player->read_pixels();
        
                   })
        .get();
}

void BanubaSdkManager::process_camera(int camera_id)
{
    if(m_realtime_processor == nullptr){
        m_realtime_processor = bnb::interfaces::frame_processor::create_realtime_processor(bnb::interfaces::realtime_processor_mode::async, m_config);
    }
    
    m_effect_player->set_frame_processor(m_realtime_processor);
    // Callback for new camera image
    auto ef_cb = [this](bnb::full_image_t image) {
        if (!m_window_is_shown) {
            const auto& format = image.get_format();
            m_window.show(format.width, format.height);
            m_render_thread->update_surface_size(format.width, format.height);
            m_window_is_shown = true;
        }
        
        static int64_t frame_number = 0;

        auto fd = bnb::interfaces::frame_data::create();
        fd->add_full_img(std::move(image));
        fd->add_frame_number(frame_number++);
        
        m_realtime_processor->push(fd);
    };
    m_camera_ptr = bnb::create_camera_device(ef_cb, camera_id);
}

void BanubaSdkManager::start_render_thread()
{
    m_render_thread = std::make_unique<RenderThread>(m_window.get_window(), *m_effect_player);
}

#if BNB_OS_WINDOWS || BNB_OS_LINUX // See BanubaSdkManager.mm for OSX
std::string BanubaSdkManager::sdk_resources_path() {
    return ".";
}
#endif
