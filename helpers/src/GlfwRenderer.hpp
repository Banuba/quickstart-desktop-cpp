#pragma once

#include <bnb/player_api/interfaces/render_delegate.hpp>
#include "GlfwWindow.hpp"

#include <memory>

class GLFWRenderer : public bnb::player_api::interfaces::render_delegate
{
public:
    GLFWRenderer()
    {
        m_window = std::make_shared<GlfwWindow>("BanubaSDK Quickstart");
        m_window->make_context_current();
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->swap_buffers();
        m_window->make_nothing_current();
    }

    ~GLFWRenderer() = default;

    std::shared_ptr<GlfwWindow> get_window()
    {
        return m_window;
    }
    
    // Called before rendering process, assume activation context
    void activate() override
    {
        m_window->make_context_current();
    }
    // Called when begin rendering
    void started() override
    {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    //Called on finish rendering
    void finished(int64_t frame_number) override
    {
        if (frame_number != -1) {
            m_window->swap_buffers();
        }
        m_window->make_nothing_current();
    }

private:
    std::shared_ptr<GlfwWindow> m_window;
}; // render_process
