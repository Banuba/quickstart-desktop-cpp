#pragma once

#include <bnb/effect_player/interfaces/effect_player.hpp>

#include <bnb/player_api/interfaces/render_delegate.hpp>
#include "GlfwWindow.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <async++.h>


class glfw_renderer : public bnb::player_api::interfaces::render_delegate
{
public:
    glfw_renderer()
    {
        m_window = std::make_shared<glfw_window>("BanubaSDK Quickstart");
        m_window->make_context_current();
        glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->swap_buffers();
        m_window->make_nothing_current();
    }

    ~glfw_renderer() = default;

    std::shared_ptr<glfw_window> get_window()
    {
        return m_window;
    }

    void activate() override
    {
        m_window->make_context_current();
    }

    void started() override
    {
        glClearColor(0.2f, 0.3f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void finished(int64_t frame_number) override
    {
        if (frame_number != -1) {
            m_window->swap_buffers();
        }
        m_window->make_nothing_current();
    }

private:
    std::shared_ptr<glfw_window> m_window;
}; // render_process
