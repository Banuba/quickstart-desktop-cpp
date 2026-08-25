set(HELPERS_SOURCES
    src/GlfwRenderer.hpp
    src/GlfwWindow.cpp
    src/GlfwWindow.hpp
    src/Utils.hpp
    src/BanubaClientToken.hpp
)

if(APPLE)
    list(APPEND HELPERS_SOURCES
        src/Utils.mm
        src/GlfwRenderer.mm
    )
else()
    list(APPEND HELPERS_SOURCES
        src/Utils.cpp
        src/GlfwRenderer.cpp
    )
endif()

add_library(helpers STATIC
    ${HELPERS_SOURCES}
)

target_link_libraries(helpers
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
