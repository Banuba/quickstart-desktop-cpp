cmake_minimum_required(VERSION 3.9)

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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/bnb_sdk_manager)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third)

option(DEPLOY_BUILD "Build for deployment" OFF)

###########
# Targets #
###########

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/realtime-camera-preview)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/single-image-processing)
