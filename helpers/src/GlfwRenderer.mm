#include "GlfwRenderer.hpp"

#include <Foundation/Foundation.h>
#include <QuartzCore/CAMetalLayer.h>
#include <MetalKit/MetalKit.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

using bnb::interfaces::render_backend_type;

void* GLFWRenderer::get_native_surface()
{
    if (m_render_backend == render_backend_type::opengl) {
        return nullptr;
    }

    NSWindow* ns_window = glfwGetCocoaWindow(m_window->get_window());
    [ns_window.contentView setWantsLayer:YES];
    CAMetalLayer* metal_layer = [CAMetalLayer layer];
    metal_layer.device = MTLCreateSystemDefaultDevice();
    metal_layer.contentsScale = ns_window.backingScaleFactor;
    [ns_window.contentView setLayer:metal_layer];
    return (__bridge void*) metal_layer;
}
