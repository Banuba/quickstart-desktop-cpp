add_executable(single-image-processing
    main.cpp
)

target_link_libraries(single-image-processing
    helpers
)

if (APPLE)
    set(EXAMPLE_BUNDLE_NAME "Banuba Single Image Processing Example") # This name can contain up to 15 characters according to Apple's documentation
    set(EXAMPLE_COPYRIGHT "© 2023 Banuba. All rights reserved.")
    set(EXAMPLE_INFO "Banuba Single Image Processing Example.")

    set_target_properties(single-image-processing PROPERTIES
        OUTPUT_NAME ${EXAMPLE_BUNDLE_NAME}
        MACOSX_BUNDLE TRUE

        # The same as stadrard MacOSXBundleInfo.plist.in from CMake but with camera permissions added
        MACOSX_BUNDLE_INFO_PLIST "${BNB_RESOURCES_FOLDER}/mac/Info.plist.in"

        MACOSX_BUNDLE_BUNDLE_NAME           ${EXAMPLE_BUNDLE_NAME}    # Sets CFBundleName
        MACOSX_BUNDLE_COPYRIGHT             ${EXAMPLE_COPYRIGHT}      # Sets NSHumanReadableCopyright
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.banuba.sdk.example.image"  # Sets CFBundleIdentifier
        MACOSX_BUNDLE_INFO_STRING           ${EXAMPLE_INFO}           # Sets CFBundleGetInfoString

        # Explicitly skip code signing (CMake tries to turn it on for application bundles)
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )
endif()

copy_sdk(single-image-processing)
copy_third(single-image-processing)
