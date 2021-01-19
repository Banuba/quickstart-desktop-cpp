#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <queue>
#include <thread>
#include <async++.h>

#include "camera_renderer.hpp"

namespace render {

class RenderThread
{
public:
    RenderThread(GLFWwindow* window);

    ~RenderThread();

    void update_context(std::shared_ptr<bnb::full_image_t> image);

    template<typename F>
    auto schedule(const F& f)
    {
        return async::spawn(m_scheduler, f);
    }

private:
    void thread_func();

    bnb::camera_renderer m_renderer;
    GLFWwindow* m_window;
    std::thread m_thread;
    std::atomic<bool> m_cancellation_flag;
    async::fifo_scheduler m_scheduler;
};

} //render