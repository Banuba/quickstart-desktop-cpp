if (APPLE)
    set(BNB_SDK_ARCH "mac")
    set(SDK_FILE_TYPE "framework")
    set(SDK_EFFECT_PLAYER_LIB "BanubaEffectPlayer")
else()
    set(BNB_SDK_ARCH "bin/x64")
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(BNB_SDK_ARCH "bin/x86")
    endif()
    set(SDK_FILE_TYPE "lib")
    set(SDK_EFFECT_PLAYER_LIB "bnb_effect_player")
endif()

set(SDK_COMMON_FOLDER ${CMAKE_CURRENT_LIST_DIR}/${BNB_SDK_ARCH})
set(SDK_TARGET bnb_effect_player)

add_library(${SDK_TARGET} INTERFACE)

set_property(TARGET ${SDK_TARGET} PROPERTY INTERFACE_BIN_DIR ${SDK_COMMON_FOLDER})

if (APPLE)
    set_property(TARGET ${SDK_TARGET} PROPERTY FRAMEWORK TRUE)

    target_link_libraries(${SDK_TARGET} INTERFACE debug "-F ${SDK_COMMON_FOLDER}/debug -framework ${SDK_EFFECT_PLAYER_LIB}")
    target_link_libraries(${SDK_TARGET} INTERFACE optimized "-F ${SDK_COMMON_FOLDER}/release -framework ${SDK_EFFECT_PLAYER_LIB}")

    target_include_directories(${SDK_TARGET} INTERFACE debug "${SDK_COMMON_FOLDER}/debug/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}/PrivateHeaders")
    target_include_directories(${SDK_TARGET} INTERFACE optimized "${SDK_COMMON_FOLDER}/release/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}/PrivateHeaders")
else()
    target_link_libraries(${SDK_TARGET} INTERFACE debug "${SDK_COMMON_FOLDER}/debug/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}")
    target_link_libraries(${SDK_TARGET} INTERFACE optimized "${SDK_COMMON_FOLDER}/release/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}")

    target_include_directories(${SDK_TARGET} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endif()
