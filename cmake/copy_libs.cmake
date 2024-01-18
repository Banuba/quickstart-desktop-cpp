function(copy_sdk target)
    if (MSVC)
        set_property(TARGET ${target} APPEND PROPERTY LINK_FLAGS /STACK:4194304)
        set(SDK_EFFECT_PLAYER_LIB "bnb_effect_player")
        set(SDK_FILE_TYPE "dll")

        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_PROPERTY:bnb_effect_player,INTERFACE_BIN_DIR>/$<CONFIG>/${SDK_EFFECT_PLAYER_LIB}.${SDK_FILE_TYPE}
                $<TARGET_FILE_DIR:${target}>
            COMMENT "Copy banuba dynamic libs"
        )
    elseif (APPLE)
        # copy BanubaEffectPlayer.framework to app bundle
        add_custom_command(TARGET ${target}
            POST_BUILD                     
            COMMAND 
                rm -rf $<TARGET_FILE_DIR:${target}>/../Frameworks
                &&
                mkdir -p $<TARGET_FILE_DIR:${target}>/../Frameworks
                &&    
                cp -rfP 
                    ${CMAKE_SOURCE_DIR}/bnb_sdk/mac/BanubaEffectPlayer.framework  
                    $<TARGET_FILE_DIR:${target}>/../Frameworks
        )
    endif ()
endfunction()

function(copy_third target)
    if (NOT MSVC)
        return()
    endif ()

    set(ARCH_SUFFIX 64)
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ARCH_SUFFIX 32)
    endif ()

    # FFPMEG
    if (BNB_VIDEO_PLAYER)
        set(FFMPEG_BIN_DIR ${CMAKE_SOURCE_DIR}/bnb_sdk/bin/ffmpeg/x${ARCH_SUFFIX})
        file(GLOB FFMPEG_LINK_LIBS LIST_DIRECTORIES false "${FFMPEG_BIN_DIR}/*.dll")
        message(INFO ${FFMPEG_BIN_DIR})

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

   
    set(OPENAL_BIN_DIR ${CMAKE_SOURCE_DIR}/bnb_sdk/bin/openal/x${ARCH_SUFFIX})

    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${OPENAL_BIN_DIR}/OpenAL32.dll
            $<TARGET_FILE_DIR:${target}>
        COMMENT "Copy OpenAL dlls to build dir"
    )

endfunction()
