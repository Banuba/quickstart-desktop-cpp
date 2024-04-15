#pragma once

#include <GLFW/glfw3.h>

#include <string_view>
#include <cstdint>
#include <functional>

enum class glfw_event_t : int32_t
{
    window_move,
    window_resize,
    window_close,

    framebuffer_resize,

    mouse_press,
    mouse_release,

    mouse_move,
    mouse_hower,
    mouse_left,

    scroll,

    key_press,
    key_repeat,
    key_release,

    input_codepoint,

    drop_files
};

struct glfw_event
{
    glfw_event_t type;
    GLFWwindow* window;
    union
    {
        double scroll_x;
        const char** file_paths;
        int32_t pos_x;
        int32_t size_width;
        int32_t keyboard_key;
        int32_t mouse_button;
        uint32_t codepoint;
    };
    union
    {
        double scroll_y;
        int32_t file_count;
        int32_t pos_y;
        int32_t size_height;
        int32_t keyboard_scancode;
        int32_t mouse_mods;
    };
    union
    {
        int32_t keyboard_mods;
    };
}; /* glfw_event */


class glfw_window
{
public:
    using glfw_event_callback = std::function<void(const glfw_event& event)>;

public:
    glfw_window(const std::string_view& title);
    ~glfw_window();

    void make_context_current();
    void make_nothing_current();
    void swap_buffers();

    void set_glfw_events_callback(glfw_event_callback callback);

    void show_window_and_run_events_loop();

    [[nodiscard]] GLFWwindow* get_window() const
    {
        return m_window;
    }

private:
    void track_events();
    void untrack_events();

private:
    GLFWwindow* m_window{nullptr};
    glfw_event_callback m_glfw_event_callback;
}; /* class glfw_window */

