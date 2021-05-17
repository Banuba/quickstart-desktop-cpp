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
    )
endif()

add_library(bnb_sdk_manager STATIC
    ${BNB_SDK_SOURCES}
)

target_link_libraries(bnb_sdk_manager
    Async++
    glad
    glfw
    stb
    opencv
    bnb_effect_player
)

if (APPLE)
    target_link_libraries(bnb_sdk_manager
        Boost::filesystem
    )
endif()

target_include_directories(bnb_sdk_manager PUBLIC src/)
