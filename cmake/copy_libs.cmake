function(copy_sdk target)
    if (MSVC)
        set_property(TARGET ${target} APPEND PROPERTY LINK_FLAGS /STACK:4194304)
        set(SDK_POSTPROCESS_LIB "bnb_postprocess")
        set(SDK_EFFECT_PLAYER_LIB "bnb_effect_player")
        set(SDK_FILE_TYPE "dll")
    elseif (APPLE)
        set(SDK_POSTPROCESS_LIB "libbnb_postprocess")
        set(SDK_EFFECT_PLAYER_LIB "libbnb_effect_player")
        set(SDK_FILE_TYPE "dylib")
    endif ()

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PROPERTY:bnb_effect_player,INTERFACE_BIN_DIR>/$<CONFIG>/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}
            $<TARGET_PROPERTY:bnb_effect_player,INTERFACE_BIN_DIR>/$<CONFIG>/${SDK_POSTPROCESS_LIB}.${SDK_FILE_TYPE}
            $<TARGET_FILE_DIR:${target}>
        COMMENT "Copy banuba dynamic libs"
    )
endfunction()

function(copy_third target)
    if (NOT MSVC)
        return()
    endif ()
    set(IS_WIN64 $<EQUAL:${CMAKE_SIZEOF_VOID_P},8>)
    set(DEBUG_SUFFIX $<$<CONFIG:Debug>:d>)

    # FFPMEG

    if (BNB_VIDEO_PLAYER)
        set(FFMPEG_ARCH_SUFFIX 64)
        if (CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(FFMPEG_ARCH_SUFFIX 32)
        endif ()
        set(FFMPEG_BIN_DIR "${BNB_THIRD_FOLDER}/ffmpeg/win${FFMPEG_ARCH_SUFFIX}/bin")
        file(GLOB FFMPEG_LINK_LIBS LIST_DIRECTORIES false "${FFMPEG_BIN_DIR}/*.dll")

        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${FFMPEG_LINK_LIBS}
                $<TARGET_FILE_DIR:${target}>
            COMMENT "Copy ffmpeg dlls to build dir"
        )
    endif ()

    # OPENAL

    set(OPENAL_ARCH_SUFFIX $<IF:${IS_WIN64},64,32>)
    set(OPENAL_BIN_DIR ${BNB_THIRD_FOLDER}/openal/bin/Win${OPENAL_ARCH_SUFFIX})

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${OPENAL_BIN_DIR}/OpenAL32.dll
            ${OPENAL_BIN_DIR}/wrap_oal.dll
            $<TARGET_FILE_DIR:${target}>
        COMMENT "Copy OpenAL dlls to build dir"
    )
endfunction()