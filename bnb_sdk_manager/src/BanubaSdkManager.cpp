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

bnb::data_t BanubaSdkManager::process_image(const std::filesystem::path& path)
{
    return m_render_thread->schedule([this, path]() {
                       //Process 1 pixel first
                       uint8_t data[] = {0, 0, 0, 0 };
                       bnb::image_format format;
                       format.orientation = bnb::camera_orientation::deg_0;
                       format.require_mirroring = false;
                       format.width = 1;
                       format.height = 1;

                       bnb::bpc8_image_t bpc_image(bnb::color_plane_weak(data), bnb::interfaces::pixel_format::rgba, format);
                       bnb::full_image_t one_pixel(std::move(bpc_image));

                       m_effect_player->process_image(
                           std::move(one_pixel),
                           bnb::interfaces::pixel_format::rgba,
                           bnb::interfaces::process_image_params(false, std::nullopt, std::nullopt));

                       auto name = path.filename().string();
                       auto img = bnb::full_image_t::load(path.string());
                       auto fmt = img.get_format();
                       auto result_img = m_effect_player->process_image(
                           std::move(img),
                           bnb::interfaces::pixel_format::rgba,
                           bnb::interfaces::process_image_params(false, std::nullopt, std::nullopt));
                       return result_img;
                   })
        .get();
}

void BanubaSdkManager::process_camera(int camera_id)
{
    // Callback for new camera image
    auto ef_cb = [this](bnb::full_image_t image) {
        if (!m_window_is_shown) {
            const auto& format = image.get_format();
            m_window.show(format.width, format.height);
            m_render_thread->update_surface_size(format.width, format.height);
            m_window_is_shown = true;
        }
        m_effect_player->push_frame(std::move(image));
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
