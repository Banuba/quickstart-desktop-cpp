#include "GlfwWindow.hpp"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
        
#import <QuartzCore/CAMetalLayer.h>

void* GlfwWindow::get_surface() const
{
    auto layer = CAMetalLayer.layer;
    NSWindow* ns_window = glfwGetCocoaWindow(m_window);
    ns_window.contentView.wantsLayer = YES;
    ns_window.contentView.layer = layer;
    return layer;
}
