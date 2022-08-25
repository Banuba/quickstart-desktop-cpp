#include "GlfwWindow.hpp"

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
        
#if !BNB_GL_BACKEND

    #import <Metal/MTLDevice.h>
    #import <QuartzCore/CAMetalLayer.h>

    struct metal_data::impl
    {
        impl(GLFWwindow* window)
            : gpu_device(MTLCreateSystemDefaultDevice())
            , command_queue([gpu_device newCommandQueue])
            , metal_layer([CAMetalLayer layer])
        {
            NSWindow* ns_window = glfwGetCocoaWindow(window);
            ns_window.contentView.wantsLayer = YES;
            ns_window.contentView.layer = metal_layer;
        }

        bnb::interfaces::surface_data get_surface_data() const
        {
            bnb::interfaces::surface_data data(
                reinterpret_cast<int64_t>(gpu_device),
                reinterpret_cast<int64_t>(command_queue),
                reinterpret_cast<int64_t>(metal_layer)
            );
            return data;
        }

        id<MTLDevice> gpu_device;
        id<MTLCommandQueue> command_queue;
        id metal_layer;
    };

    metal_data::metal_data(GLFWwindow* window): p_impl(std::make_unique<impl>(window))
    {
    }

    metal_data::~metal_data() = default;

    bnb::interfaces::surface_data metal_data::get_surface_data() const
    {
        return p_impl->get_surface_data();
    }

    bnb::interfaces::surface_data GlfwWindow::get_surface_data() const
    {
        return m_metal_data->get_surface_data();
    }

#endif
