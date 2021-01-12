set(include_dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/
)

file(GLOB_RECURSE srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(offscreen_ep STATIC ${srcs})

target_include_directories(offscreen_ep PUBLIC
    ${include_dirs}
)

target_link_libraries(offscreen_ep
    bnb_sdk_manager
)