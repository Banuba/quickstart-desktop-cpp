#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <async++.h>

class GlfwWindow
{
public:
    explicit GlfwWindow(const std::string& title);
    ~GlfwWindow();

    void show(uint32_t width_hint, uint32_t height_hint);
    void run_main_loop();

    [[nodiscard]] GLFWwindow* get_window() const
    {
        return m_window;
    }
    
    [[nodiscard]] void* get_surface() const;

private:
    // To execute scheduled tasks wake up main loop (glfwPostEmptyEvent)
    async::fifo_scheduler m_scheduler;
    GLFWwindow* m_window{};

    void init();
    void create_window(const std::string& title);
    void load_glad_functions();
};
