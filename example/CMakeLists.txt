file(GLOB_RECURSE srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

file(GLOB_RECURSE headers
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

add_executable(example ${srcs})

target_include_directories(example PUBLIC
    ${include_dirs}
)

target_include_directories(example SYSTEM PUBLIC ${LIBYUV_INCLUDE_DIRS})

target_link_libraries(example
    bnb_sdk_manager
    offscreen_ep
    yuv
)

if (APPLE)
    set(EXAMPLE_RESOURCES ${BNB_RESOURCES_FOLDER}/mac/bnb_viewer.icns)
    target_sources(example PRIVATE
        ${EXAMPLE_RESOURCES}
    )
    set(EXAMPLE_BUNDLE_NAME "Banuba Realtime Camera Perview Example") # This name can contain up to 15 characters according to Apple's documentation
    set(EXAMPLE_COPYRIGHT "© 2020 Banuba. All rights reserved.")
    set(EXAMPLE_INFO "Banuba Realtime Camera Perview Example.")

    set_target_properties(example PROPERTIES
        OUTPUT_NAME ${EXAMPLE_BUNDLE_NAME}
        RESOURCE ${EXAMPLE_RESOURCES}
        MACOSX_BUNDLE TRUE

        # The same as stadrard MacOSXBundleInfo.plist.in from CMake but with camera permissions added
        MACOSX_BUNDLE_INFO_PLIST "${BNB_RESOURCES_FOLDER}/mac/Info.plist.in"

        MACOSX_BUNDLE_BUNDLE_NAME           ${EXAMPLE_BUNDLE_NAME}    # Sets CFBundleName
        MACOSX_BUNDLE_COPYRIGHT             ${EXAMPLE_COPYRIGHT}      # Sets NSHumanReadableCopyright
        MACOSX_BUNDLE_GUI_IDENTIFIER        "com.banuba.sdk.example"  # Sets CFBundleIdentifier
        MACOSX_BUNDLE_ICON_FILE             bnb_viewer.icns           # Sets CFBundleIconFile
        MACOSX_BUNDLE_INFO_STRING           ${EXAMPLE_INFO}           # Sets CFBundleGetInfoString

        # Explicitly skip code signing (CMake tries to turn it on for application bundles)
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )
endif()

copy_sdk(example)
copy_third(example)