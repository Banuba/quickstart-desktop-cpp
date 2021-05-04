add_executable(single-image-processing
    main.cpp
)

target_link_libraries(single-image-processing
    bnb_sdk_manager
)

if (APPLE)
    set(EXAMPLE_RESOURCES ${BNB_RESOURCES_FOLDER}/mac/bnb_viewer.icns)
    target_sources(single-image-processing PRIVATE
        ${EXAMPLE_RESOURCES}
    )
    set(EXAMPLE_BUNDLE_NAME "Banuba Single Image Processing Example") # This name can contain up to 15 characters according to Apple's documentation
    set(EXAMPLE_COPYRIGHT "© 2021 Banuba. All rights reserved.")
    set(EXAMPLE_INFO "Banuba Single Image Processing Example.")

    set_target_properties(single-image-processing PROPERTIES
        OUTPUT_NAME ${EXAMPLE_BUNDLE_NAME}
        RESOURCE ${EXAMPLE_RESOURCES}
        MACOSX_BUNDLE TRUE

        # The same as stadrard MacOSXBundleInfo.plist.in from CMake but with camera permissions added
        MACOSX_BUNDLE_INFO_PLIST "${BNB_RESOURCES_FOLDER}/mac/Info.plist.in"

        MACOSX_BUNDLE_BUNDLE_NAME           ${EXAMPLE_BUNDLE_NAME}    # Sets CFBundleName
        MACOSX_BUNDLE_COPYRIGHT             ${EXAMPLE_COPYRIGHT}      # Sets NSHumanReadableCopyright
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.banuba.sdk.example.image"  # Sets CFBundleIdentifier
        MACOSX_BUNDLE_ICON_FILE             bnb_viewer.icns           # Sets CFBundleIconFile
        MACOSX_BUNDLE_INFO_STRING           ${EXAMPLE_INFO}           # Sets CFBundleGetInfoString

        # Explicitly skip code signing (CMake tries to turn it on for application bundles)
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )
 
    # copy BanubaEffectPlayer.framework to app bundle
    add_custom_command(TARGET single-image-processing
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bnb_sdk/mac/$<CONFIG>/BanubaEffectPlayer.framework
            $<TARGET_FILE_DIR:single-image-processing>/../Frameworks/BanubaEffectPlayer.framework
    )
    
    # This dependency is optional, you may remove it. See banuba_sdk/CMakeList.txt
    add_custom_command(TARGET single-image-processing
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/bnb_sdk/mac/$<CONFIG>/BanubaPostprocess.framework
            $<TARGET_FILE_DIR:single-image-processing>/../Frameworks/BanubaPostprocess.framework
    )
endif()

copy_sdk(single-image-processing)
copy_third(single-image-processing)
