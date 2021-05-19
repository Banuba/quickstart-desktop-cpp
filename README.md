Quick start examples for integrating [Banuba SDK on Desktop](https://docs.banuba.com/face-ar-sdk/core/effect_player) in C++ apps.

# Getting Started

1. Get the latest Banuba SDK archive for MacOS/Windows and the client token. Please fill in our form on [form on banuba.com](https://www.banuba.com/face-filters-sdk) website, or contact us via [info@banuba.com](mailto:info@banuba.com).
2. Copy `bnb_viewer_standalone/bnb_sdk/` into `quickstart-desktop-cpp/bnb_sdk` dir:
    `bnb_viewer_standalone/bnb_sdk/` => `quickstart-desktop-cpp/bnb_sdk`
3. For Windows and Linux: copy `bnb_viewer_standalone/resources/` files into `quickstart-desktop-cpp/resources` dir:
    `bnb_viewer_standalone/resources/` => `quickstart-desktop-cpp/resources`.
   For MacOS: copy `bnb_viewer_standalone/resources/mac` files into `quickstart-desktop-cpp/resources/mac` dir:
    `bnb_viewer_standalone/resources/mac` => `quickstart-desktop-cpp/resources/mac`.
4. Copy `bnb_viewer_standalone/third` files into `quickstart-desktop-cpp/third` dir:
    `bnb_viewer_standalone/third/` => `quickstart-desktop-cpp/third`
5. Copy and Paste your client token into appropriate section of `quickstart-desktop-cpp/bnb_sdk_manager/BanubaClientToken.hpp`. 

6. Generate project files by executing next commands:
    ##### Windows x86 build:	
    ```
        cd %path_to_repository%
        mkdir build
        cd build
        cmake -A Win32 ..
    ```
    ##### Windows x64 build:	
    ```
        cd %path_to_repository%
        mkdir build
        cd build
        cmake -A x64 ..
    ```
    ##### MacOS build:	
    ```
        cd %path_to_repository%
        mkdir build
        cd build
        cmake -G Xcode ..
    ```
7. Previous step will generate Xcode project for MacOS and Visual Studio project for Windows. Open viewer_standalone project in appropriate IDE on your platform.
8. Select target `single-image-processingr` or `realtime-camera-preview`.
9. Run build.

# Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
