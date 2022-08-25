#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <async++.h>

#if !BNB_GL_BACKEND
    #include <bnb/effect_player/interfaces/surface_data.hpp>

    struct metal_data
    {
        metal_data() = delete;
        metal_data(GLFWwindow* window);
        ~metal_data();

        bnb::interfaces::surface_data get_surface_data() const;

        struct impl;
        std::unique_ptr<impl> p_impl;
    };
#endif

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
    
#if !BNB_GL_BACKEND
    [[nodiscard]] bnb::interfaces::surface_data get_surface_data() const;
#endif

private:
    // To execute scheduled tasks wake up main loop (glfwPostEmptyEvent)
    async::fifo_scheduler m_scheduler;
    GLFWwindow* m_window{};
#if !BNB_GL_BACKEND
    std::unique_ptr<metal_data> m_metal_data;
#endif
    void init();
    void create_window(const std::string& title);
    void load_glad_functions();
};
