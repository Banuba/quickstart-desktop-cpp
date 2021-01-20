#include "render_thread.hpp"

#include <iostream>

#include "../../offscreen_effect_player/include/utils.hpp"

#include "opencv2/highgui/highgui.hpp"

namespace render {

RenderThread::RenderThread(GLFWwindow* window, int32_t width, int32_t height)
    : m_window(window)
    , m_thread([this]() { thread_func(); })
    , m_cancellation_flag(false)
    , m_cur_width(width)
    , m_cur_height(height)
{
}

RenderThread::~RenderThread()
{
    m_cancellation_flag = true;
    m_thread.join();
}

void RenderThread::surface_changed(int32_t width, int32_t height)
{
    m_cur_width = width;
    m_cur_height = height;

    if (m_renderer) {
        m_renderer->surface_changed(width, height);
    }
}

void RenderThread::update_data(bnb::data_t data)
{
    bnb::image_format image_f((uint32_t)m_cur_width, (uint32_t)m_cur_height, bnb::camera_orientation::deg_0, false, 0, std::nullopt);

    bnb::bpc8_image_t bpc8(
        bnb::color_plane_weak(data.data.get()),
        bnb::bpc8_image_t::pixel_format_t::rgba,
        image_f);

    cur_y_plane = convert_bpc8_to_y_plane(bpc8, image_f.width, image_f.height);
    cur_uv_plane = convert_bpc8_to_uv_plane(bpc8, image_f.width, image_f.height, false);
}

void RenderThread::update_context()
{
    if (m_renderer) {
        m_renderer->update_camera_texture(cur_y_plane, cur_uv_plane);
    }
}

void RenderThread::thread_func()
{
    using namespace std::chrono_literals;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    m_renderer = std::make_unique<bnb::renderer>(m_cur_width, m_cur_height);

    while (!m_cancellation_flag) {
        m_scheduler.run_all_tasks();
        const bool need_swap{ m_renderer->draw() != -1 };

        if (need_swap) {
            glfwSwapBuffers(m_window);
        } else {
            std::this_thread::sleep_for(1ms);
        }
    }

    glfwMakeContextCurrent(nullptr);
}

} //render