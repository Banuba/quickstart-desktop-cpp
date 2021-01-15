#ifndef CAMERA_RENDERER_HPP
#define CAMERA_RENDERER_HPP

#include <bnb/types/frame_data.hpp>

#include "renderer_gl_context.hpp"


namespace bnb {

class camera_renderer : protected logged_object
{
public:
    camera_renderer();
    virtual ~camera_renderer() override;

    void update_context(const frame_data& frame);
    void draw();
    void release();

    //
    // Methods
    //

protected:
    static void clear_gl_vertex_data(renderer_gl_context *context);

private:
    void initialize_vertex_data();
    void initialize_shader_data();
    void update_camera_texture(const frame_data& frame);

    //
    // Members
    //

private:
    std::unique_ptr<renderer_gl_context> m_cam_image_context;

};

}

#endif // CAMERA_RENDERER_HPP
