cmake_minimum_required(VERSION 3.10)

project(quickstart-desktop-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

set(BNB_RESOURCES_FOLDER ${CMAKE_CURRENT_LIST_DIR}/resources)
set(BNB_THIRD_FOLDER ${CMAKE_CURRENT_LIST_DIR}/third)

# Set to OFF to disable ffmpeg dependency (SDK should be built with disabled video_player also)
set(BNB_VIDEO_PLAYER ON)

# Careffuly tested on OSX. Better to stay with GL on other platfoems
if(APPLE)
    set(BNB_GL_BACKEND OFF)
else()
    set(BNB_GL_BACKEND ON)
endif()
#TODO: metal
set(BNB_GL_BACKEND ON)

add_definitions(
    -DBNB_RESOURCES_FOLDER="${BNB_RESOURCES_FOLDER}"
    -DBNB_VIDEO_PLAYER=$<BOOL:${BNB_VIDEO_PLAYER}>
    -DBNB_GL_BACKEND=$<BOOL:${BNB_GL_BACKEND}>
)


include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/copy_libs.cmake)

#
# Main
#

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/bnb_sdk)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/helpers)

# The samples only use the GLFW library.  Keeping its documentation out of the
# default build avoids running a bundled, old Doxygen configuration.
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third/glfw)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third/stb)

# Keep diagnostics enabled for the samples while silencing two known warnings
# in the unmodified, externally maintained stb submodule.
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(stb PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-Wno-tautological-compare>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-shorten-64-to-32>
    )
endif()

option(DEPLOY_BUILD "Build for deployment" OFF)

###########
# Targets #
###########

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/realtime-camera-preview)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/single-image-processing)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/videostream-processing)
