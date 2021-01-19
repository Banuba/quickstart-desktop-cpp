#pragma once

#include "renderer_gl_context.hpp"

#include <bnb/types/full_image.hpp>

namespace bnb {

class camera_renderer
{
public:
    camera_renderer();
    virtual ~camera_renderer();

    void update_context(std::shared_ptr<bnb::full_image_t> image);
    int draw();
    void release();

protected:
    static void clear_gl_vertex_data(renderer_gl_context *context);

private:
    void initialize_vertex_data();
    void initialize_shader_data();
    void update_camera_texture(std::shared_ptr<bnb::full_image_t> image);

private:
    std::unique_ptr<renderer_gl_context> m_gl_context;
    std::atomic<bool> m_need_draw = false;

    color_plane cur_y_plane = nullptr;
    color_plane cur_uv_plane = nullptr;
};

} // bnb