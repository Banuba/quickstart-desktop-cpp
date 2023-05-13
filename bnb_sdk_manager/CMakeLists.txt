set(BNB_SDK_SOURCES     
    src/BanubaSdkManager.cpp
    src/BanubaSdkManager.hpp
    src/RenderThread.cpp
    src/RenderThread.hpp
    src/GlfwWindow.cpp
    src/GlfwWindow.hpp
    src/BanubaClientToken.hpp
)

if(APPLE)
    list(APPEND BNB_SDK_SOURCES 
        src/BanubaSdkManager.mm
        src/GlfwWindow.mm
    )
endif()

add_library(bnb_sdk_manager STATIC
    ${BNB_SDK_SOURCES}
)

target_link_libraries(bnb_sdk_manager
    Async++
    glfw
    stb
    bnb_effect_player
)
if(BNB_GL_BACKEND)
	target_link_libraries(bnb_sdk_manager opengl32)
endif()

if (APPLE)
    target_link_libraries(bnb_sdk_manager
        "-framework Metal"
        "-framework QuartzCore"
    )
endif()

target_include_directories(bnb_sdk_manager PUBLIC src/)
