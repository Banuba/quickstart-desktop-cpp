#pragma once

#include <bnb/effect_player/interfaces/effect_player.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <queue>
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

    void add_read_pixels_callback(std::function<void()> callback);

private:
    void thread_func();
    void run_read_pixels_callback();

    GLFWwindow* m_window;
    bnb::interfaces::effect_player& m_effect_player;
    std::thread m_thread;
    std::atomic<bool> m_cancellation_flag;
    async::fifo_scheduler m_scheduler;

    std::queue<std::function<void()>> read_pixels_callbacks;
};
