add_library(bnb_sdk_manager STATIC
    src/BanubaSdkManager.cpp
    src/BanubaSdkManager.hpp
    src/RenderThread.cpp
    src/RenderThread.hpp
    src/GlfwWindow.cpp
    src/GlfwWindow.hpp
)

target_link_libraries(bnb_sdk_manager
    Async++
    glad
    glfw
    stb
    opencv
    bnb_effect_player
)

target_include_directories(bnb_sdk_manager PUBLIC src/)