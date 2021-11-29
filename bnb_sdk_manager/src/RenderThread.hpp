#pragma once

#include <bnb/effect_player/interfaces/effect_player.hpp>

#include <GLFW/glfw3.h>

#include <thread>
#include <async++.h>

class RenderThread
{
public:
    RenderThread(GLFWwindow* window, bnb::interfaces::effect_player& ep);

    ~RenderThread();

    void update_surface_size(int width, int height);

    template<typename F>
    auto schedule(const F& f)
    {
        return async::spawn(m_scheduler, f);
    }

private:
    void thread_func();

    GLFWwindow* m_window;
    bnb::interfaces::effect_player& m_effect_player;
    std::thread m_thread;
    std::atomic<bool> m_cancellation_flag;
    async::fifo_scheduler m_scheduler;
};
