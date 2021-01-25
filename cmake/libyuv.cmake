cmake_minimum_required(VERSION 3.5)

include(ExternalProject)


ExternalProject_Add(
  libyuv
  GIT_REPOSITORY https://github.com/lemenkov/libyuv.git
  GIT_TAG 19d71f6b351fe992ae34b114eebd872c383a6bdb
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/libyuv/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON)

ExternalProject_Get_Property(libyuv source_dir)
set(LIBYUV_INCLUDE_DIRS ${source_dir}/include)

ExternalProject_Get_Property(libyuv binary_dir)
set(LIBYUV_LIBRARY_PATH ${binary_dir})
set(LIBYUV_LIBRARY yuv)
add_library(${LIBYUV_LIBRARY} UNKNOWN IMPORTED)
if(CMAKE_GENERATOR STREQUAL Xcode)
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}libyuv.a
    IMPORTED_LOCATION_RELEASE ${LIBYUV_LIBRARY_PATH}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}libyuv.a
    IMPORTED_LOCATION_DEBUG ${LIBYUV_LIBRARY_PATH}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}libyuv.a
    )
elseif(MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
      IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}/Debug/yuv.lib
      )
  else()
    set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
      IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}/Release/yuv.lib
      )
  endif()
else()
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}libyuv.a
    )
endif()

add_dependencies(${LIBYUV_LIBRARY} libyuv)