add_executable(single-image-processing
    main.cpp
)

target_link_libraries(single-image-processing
    bnb_sdk_manager
)

copy_sdk(single-image-processing)
copy_third(single-image-processing)