#include "GlfwRenderer.hpp"

#include <bnb/utils/defs.hpp>

#if !BNB_APPLE // See GlfwRenderer.mm for MacOS
void* GLFWRenderer::get_native_surface()
{
    return nullptr;
}
#endif