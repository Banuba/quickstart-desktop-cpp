#pragma once

#include "frame_surface_handler.hpp"

namespace bnb {

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
