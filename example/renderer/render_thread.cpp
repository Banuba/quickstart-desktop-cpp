#include "render_thread.hpp"

#include <iostream>

namespace render {

RenderThread::RenderThread(GLFWwindow* window)
    : m_window(window)
    , m_thread([this]() { thread_func(); })
    , m_cancellation_flag(false)
{
}

RenderThread::~RenderThread()
{
    m_cancellation_flag = true;
    m_thread.join();
}

void RenderThread::update_context(std::shared_ptr<bnb::full_image_t> image)
{
    m_renderer.update_context(image);
}

void RenderThread::thread_func()
{
    using namespace std::chrono_literals;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    while (!m_cancellation_flag) {
        m_scheduler.run_all_tasks();
        const bool need_swap{ m_renderer.draw() != -1 };

        if (need_swap) {
            std::cout << "swap" << std::endl;
            glfwSwapBuffers(m_window);
        } else {
            std::this_thread::sleep_for(1ms);
        }
    }

    glfwMakeContextCurrent(nullptr);
}

} //render