#include "GlfwWindow.hpp"

#include <math.h>
#include <assert.h>
#include <stdio.h>

#include <bnb/effect_player/utility.hpp>
#include <stdexcept>

using bnb::interfaces::render_backend_type;

namespace
{

    void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    void calculate_window_size_and_pos(int32_t& x, int32_t& y, int32_t& w, int32_t& h)
    {
        //
        // All calculation are based on GLFW's screen coordinates
        // so result behaviour is pretty similar on both retina and
        // non-retina screens
        //

        GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();

        if (primary_monitor == nullptr) {
            throw std::runtime_error("glfwGetPrimaryMonitor() error");
        }

        const GLFWvidmode* current_video_mode = glfwGetVideoMode(primary_monitor);

        if (current_video_mode == nullptr) {
            throw std::runtime_error("glfwGetVideoMode() error");
        }

        auto screen_height = static_cast<float>(current_video_mode->height);
        auto screen_width = static_cast<float>(current_video_mode->width);

        // Chose the window size so that it occupies the 3/4 of the screen's width.
        // Then shrink it if the height is too high. Aspect calculated by the hints is
        // always preserved.

        constexpr float aspect = 16.0f / 9.0f;

        float scaled_w = 0.8f * screen_width;
        float scaled_h = scaled_w / aspect;

        // put in the center of the screen
        x = static_cast<int32_t>(std::max((screen_width - scaled_w), 0.f) / 2.f);
        y = static_cast<int32_t>(std::max((screen_height - scaled_h), 0.f) / 2.f);
        w = static_cast<int32_t>(scaled_w);
        h = static_cast<int32_t>(scaled_h);
    }

} /* namespace */

GlfwWindow::GlfwWindow(const std::string_view& title, render_backend_type render_backend)
    : m_render_backend(render_backend)
{
    // Init glfw for glfw_window
    if (!glfwInit()) {
        throw std::runtime_error("glfwInit() error");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    if (m_render_backend == render_backend_type::opengl) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    if ((m_window = glfwCreateWindow(1280, 720, title.data(), nullptr, nullptr)) == nullptr) {
        throw std::runtime_error("glfwCreateWindow() error");
    }

    glfwSetErrorCallback(glfw_error_callback);

    if (m_render_backend == render_backend_type::opengl) {
        glfwMakeContextCurrent(m_window);
        bnb::utility::load_gl_functions();
    }

    glfwSwapInterval(0);

    track_events();
}

GlfwWindow::~GlfwWindow()
{
    untrack_events();
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/* glfw_window::make_context_current */
void GlfwWindow::make_context_current()
{
    if (m_render_backend == render_backend_type::opengl) {
        glfwMakeContextCurrent(m_window);
    }
}

/* glfw_window::make_nothing_current */
void GlfwWindow::make_nothing_current()
{
    if (m_render_backend == render_backend_type::opengl) {
        glfwMakeContextCurrent(nullptr);
    }
}

/* glfw_window::swap_buffers */
void GlfwWindow::swap_buffers()
{
    if (m_render_backend == render_backend_type::opengl) {
        glfwSwapBuffers(m_window);
    }
}

/* glfw_window::show_window_and_run_events_loop */
void GlfwWindow::show_window_and_run_events_loop()
{
    int32_t x, y, w, h;
    calculate_window_size_and_pos(x, y, w, h);
    glfwSetWindowPos(m_window, x, y);
    glfwSetWindowSize(m_window, w, h);
    glfwShowWindow(m_window);

    while (!glfwWindowShouldClose(m_window)) {
        glfwWaitEvents();
    }
}

/* glfw_window::track_events */
void GlfwWindow::track_events()
{
    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        if(auto ptr = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))){
            if(ptr->m_close_callback){
                ptr->m_close_callback();
            }
        }
    });
    
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        if(auto ptr = reinterpret_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))){
            if(ptr->m_resize_callback){
                ptr->m_resize_callback(width, height);
            }
        }
    });
}

void GlfwWindow::untrack_events()
{
    glfwSetWindowUserPointer(m_window, nullptr);
    glfwSetWindowCloseCallback(m_window, nullptr);
    glfwSetFramebufferSizeCallback(m_window, nullptr);
}
