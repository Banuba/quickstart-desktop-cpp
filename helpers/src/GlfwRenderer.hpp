#pragma once

#include <bnb/player_api/interfaces/render_delegate.hpp>
#include <bnb/scene/interfaces/render_backend_type.hpp>
#include "GlfwWindow.hpp"

#include <memory>

class GLFWRenderer : public bnb::player_api::interfaces::render_delegate
{
public:
    GLFWRenderer(bnb::interfaces::render_backend_type render_backend)
        : m_render_backend(render_backend)
    {
        m_window = std::make_shared<GlfwWindow>("BanubaSDK Quickstart", m_render_backend);
        m_window->make_context_current();
        m_window->swap_buffers();
        m_window->make_nothing_current();
    }

    ~GLFWRenderer() = default;

    std::shared_ptr<GlfwWindow> get_window()
    {
        return m_window;
    }

    void* get_native_surface();

    // Called before rendering process, assume activation context
    void activate() override
    {
        m_window->make_context_current();
    }

    // Called when begin rendering
    void started() override
    {
    }

    //Called on finish rendering
    void finished(int64_t frame_number) override
    {
        if (frame_number != -1) {
            m_window->swap_buffers();
        }
    }

private:
    std::shared_ptr<GlfwWindow> m_window;
    bnb::interfaces::render_backend_type m_render_backend;
}; // render_process
