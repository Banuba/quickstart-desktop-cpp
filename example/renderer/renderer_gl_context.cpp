#include "renderer_gl_context.hpp"

using namespace bnb;

renderer_gl_context::renderer_gl_context()
    : m_frame_surface(camera_orientation::deg_0)
    , texture_uniform_location {0, 0}
    , textures {0, 0}
{
}

renderer_gl_context::~renderer_gl_context()
{
    if (std::any_of(textures, textures + TexturesAmount, [](unsigned int a) -> bool {return a != 0;})) {
        glDeleteTextures(renderer_gl_context::TexturesAmount, textures);
    }
    std::fill_n(textures, TexturesAmount, 0);
}
