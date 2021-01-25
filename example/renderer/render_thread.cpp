#include "render_thread.hpp"

#include <chrono>
#include <iostream>
#include <libyuv.h>

namespace {

using namespace std::chrono;

void get_time(const std::string& des)
{
    auto ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    ).count();
    std::cout << des << ": " << ms << std::endl;
}

}

namespace render {

RenderThread::RenderThread(GLFWwindow* window, int32_t width, int32_t height)
    : m_window(window)
    , m_thread([this]() { thread_func(); })
    , m_cancellation_flag(false)
    , m_cur_width(width)
    , m_cur_height(height)
{
    m_y_row_ptr = (uint8_t*) malloc(m_cur_height * m_cur_width * 4 * sizeof(uint8_t));
    m_uv_row_ptr = (uint8_t*) malloc(m_cur_height * m_cur_width * 2 * sizeof(uint8_t));

    m_cur_y_plane.reset(m_y_row_ptr);
    m_cur_uv_plane.reset(m_uv_row_ptr);
}

RenderThread::~RenderThread()
{
    delete m_y_row_ptr;
    delete m_uv_row_ptr;

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

    libyuv::ABGRToNV12(data.data.get(),
               m_cur_width * 4,
               m_cur_y_plane.get(),
               m_cur_width,
               m_cur_uv_plane.get(),
               m_cur_width,
               m_cur_width,
               m_cur_height);
}

void RenderThread::update_context()
{
    if (m_renderer) {
        m_renderer->update_camera_texture(m_cur_y_plane, m_cur_uv_plane);
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
            std::this_thread::sleep_for(1us);
        }
    }

    glfwMakeContextCurrent(nullptr);
}

} //render