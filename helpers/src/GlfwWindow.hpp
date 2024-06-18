#pragma once

#include <bnb/scene/interfaces/render_backend_type.hpp>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <functional>
#include <string_view>

class GlfwWindow
{
public:
    using resize_callback = std::function<void(uint32_t, uint32_t)>;
    using close_callback = std::function<void()>;

public:
    GlfwWindow(const std::string_view& title, bnb::interfaces::render_backend_type render_backend);
    ~GlfwWindow();

    void make_context_current();
    void make_nothing_current();
    void swap_buffers();

    void show_window_and_run_events_loop();

    [[nodiscard]] GLFWwindow* get_window() const
    {
        return m_window;
    }
    
    void set_callbacks(resize_callback resize, close_callback close){
        m_resize_callback = std::move(resize);
        m_close_callback = std::move(close);
    }

private:
    void track_events();
    void untrack_events();

private:
    GLFWwindow* m_window{nullptr};
    resize_callback m_resize_callback;
    close_callback m_close_callback;
    bnb::interfaces::render_backend_type m_render_backend;
}; /* class glfw_window */

