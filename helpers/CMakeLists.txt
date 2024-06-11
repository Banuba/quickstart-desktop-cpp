set(HELPERS_SOURCES
    src/GlfwRenderer.cpp
    src/GlfwRenderer.hpp
    src/GlfwWindow.cpp
    src/GlfwWindow.hpp
    src/Utils.hpp
    src/Utils.cpp
    src/BanubaClientToken.hpp
)

if(APPLE)
    list(APPEND HELPERS_SOURCES
        src/Utils.mm
        src/GlfwRenderer.mm
    )
endif()

add_library(helpers STATIC
    ${HELPERS_SOURCES}
)

target_link_libraries(helpers
    Async++
    glfw
    stb
    bnb_effect_player
)
if (APPLE)
    target_link_libraries(helpers
        "-framework Metal"
        "-framework QuartzCore"
        "-framework OpenGL"
    )
else()
    target_link_libraries(helpers opengl32)
endif()

target_include_directories(helpers PUBLIC src/)
