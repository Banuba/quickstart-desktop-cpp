#include "RenderThread.hpp"

#include <bnb/effect_player/interfaces/all.hpp>

RenderThread::RenderThread(GLFWwindow* window, bnb::interfaces::effect_player& ep)
    : m_window(window)
    , m_effect_player(ep)
    , m_thread([this]() { thread_func(); })
    , m_cancellation_flag(false)
{
}

RenderThread::~RenderThread()
{
    m_cancellation_flag = true;
    m_thread.join();
}

void RenderThread::update_surface_size(int width, int height)
{
    async::spawn(
        m_scheduler,
        [this, width, height]() {
            m_effect_player.surface_changed(width, height);
            m_effect_player.effect_manager()->set_effect_size(width, height);
        });
}

void RenderThread::thread_func()
{
    using namespace std::chrono_literals;

#if BNB_GL_BACKEND
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
#endif

    m_effect_player.surface_created(720, 1280);

    while (!m_cancellation_flag) {
        m_scheduler.run_all_tasks();
        
        auto [status, processed_fd] = m_effect_player.frame_processor()->pop();
        bool need_swap = false;
        if (processed_fd) {
            auto draw_result = m_effect_player.draw_with_external_frame_data(processed_fd);
            need_swap = draw_result != -1;
        }

        if (need_swap) {
#if BNB_GL_BACKEND
            glfwSwapBuffers(m_window);
#endif
        } else {
            // On effect change active_recognizer::pop_frame_data() will continuosly lock _pop_mutex,
            // m_effect_player.draw() will return false as there is nothing to draw, this loop will skip wait
            // on glfwSwapBuffers and without this sleep active_recognizer::clear() may wait for
            // _pop_mutex forever (at least on Win).
            std::this_thread::sleep_for(1us);
        }
    }
    m_effect_player.effect_manager()->load("");
    m_effect_player.surface_destroyed();
    
#if BNB_GL_BACKEND
    glfwMakeContextCurrent(nullptr);
#endif
}
