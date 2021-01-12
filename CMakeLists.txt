cmake_minimum_required(VERSION 3.9)

project(quickstart-desktop-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12")

set(BNB_RESOURCES_FOLDER ${CMAKE_CURRENT_LIST_DIR}/resources)
set(BNB_THIRD_FOLDER ${CMAKE_CURRENT_LIST_DIR}/third)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_ENABLE_EXTENDED_ALIGNED_STORAGE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_ENABLE_EXTENDED_ALIGNED_STORAGE")

# Set to OFF to disable ffmpeg dependency (SDK should be built with disabled video_player also)
set(BNB_VIDEO_PLAYER ON)

add_definitions(
    -DBNB_RESOURCES_FOLDER="${BNB_RESOURCES_FOLDER}"
    -DBNB_VIDEO_PLAYER=$<BOOL:${BNB_VIDEO_PLAYER}>
)


include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/copy_libs.cmake)

#
# Main
#

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/bnb_sdk)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/bnb_sdk_manager)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/offscreen_effect_player)

option(DEPLOY_BUILD "Build for deployment" OFF)

###########
# Targets #
###########

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/realtime-camera-preview)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/single-image-processing)
