add_executable(realtime-camera-preview
    main.cpp
)

target_link_libraries(realtime-camera-preview
    bnb_sdk_manager
)

if (APPLE)
    set(EXAMPLE_BUNDLE_NAME "Banuba Realtime Camera Perview Example") # This name can contain up to 15 characters according to Apple's documentation
    set(EXAMPLE_COPYRIGHT "© 2023 Banuba. All rights reserved.")
    set(EXAMPLE_INFO "Banuba Realtime Camera Perview Example.")

    set_target_properties(realtime-camera-preview PROPERTIES
        OUTPUT_NAME ${EXAMPLE_BUNDLE_NAME}
        MACOSX_BUNDLE TRUE

        # The same as stadrard MacOSXBundleInfo.plist.in from CMake but with camera permissions added
        MACOSX_BUNDLE_INFO_PLIST "${BNB_RESOURCES_FOLDER}/mac/Info.plist.in"

        MACOSX_BUNDLE_BUNDLE_NAME           ${EXAMPLE_BUNDLE_NAME}    # Sets CFBundleName
        MACOSX_BUNDLE_COPYRIGHT             ${EXAMPLE_COPYRIGHT}      # Sets NSHumanReadableCopyright
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.banuba.sdk.example"  # Sets CFBundleIdentifier
        MACOSX_BUNDLE_INFO_STRING           ${EXAMPLE_INFO}           # Sets CFBundleGetInfoString

        # Explicitly skip code signing (CMake tries to turn it on for application bundles)
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )
 
    # copy BanubaEffectPlayer.framework to app bundle
    add_custom_command(TARGET realtime-camera-preview 
        POST_BUILD 					   
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${CMAKE_SOURCE_DIR}/bnb_sdk/mac/$<CONFIG>/BanubaEffectPlayer.framework  
            $<TARGET_FILE_DIR:realtime-camera-preview>/../Frameworks/BanubaEffectPlayer.framework
    )
endif()

copy_sdk(realtime-camera-preview)
copy_third(realtime-camera-preview)
