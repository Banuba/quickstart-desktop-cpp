#pragma once

#include <bnb/types/base_types.hpp>

namespace bnb {

    struct frame_surface_handler
    {
        camera_orientation m_orientation = camera_orientation::deg_90;
        int shader_program = 0;
        unsigned int vbo = 0;
        unsigned int vao = 0;
        unsigned int ebo = 0;

        frame_surface_handler(camera_orientation orientation)
        : m_orientation(orientation)
        {}
    };

    class renderer_gl_context
    {
    public:
        renderer_gl_context();
        ~renderer_gl_context();

        static constexpr int TexturesAmount = 2;

        frame_surface_handler m_frame_surface;

        int             texture_uniform_location[TexturesAmount];
        unsigned int    textures[TexturesAmount];
    };

    namespace SamplerName {
        static const char * Y = "yTexture";
        static const char * UV = "uvTexture";
    }

    namespace SamplerIndex {
        static const int Y = 0;
        static const int UV = 1;
    }

}
