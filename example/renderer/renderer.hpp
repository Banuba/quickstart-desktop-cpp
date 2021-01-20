#pragma once

#include <cstdint>
#include <vector>
#include <queue>

#include <bnb/types/full_image.hpp>

#include "program.hpp"
#include "renderer_gl_context.hpp"

namespace bnb
{
    class renderer
    {
    public:
        renderer(int width, int height);

        void surface_changed(int32_t width, int32_t height);
        void update_camera_texture(color_plane y_plane, color_plane uv_plane);
        int draw();

    private:
        renderer_gl_context m_gl_context;
        program m_camera_program;

        color_plane cur_y_plane = nullptr;
        color_plane cur_uv_plane = nullptr;

        int cur_widht;
        int cur_height;

        std::atomic<bool> m_need_draw = false;
    };
}
