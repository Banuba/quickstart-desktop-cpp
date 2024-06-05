[![](https://www.banuba.com/hubfs/Banuba_November2018/Images/Banuba%20SDK.png)](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview)

## Quick start examples for integrating [Banuba SDK on Desktop](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview) in C++ apps.  

## [Requirements](https://docs.banuba.com/face-ar-sdk-v1/overview/system_requirements)

## Usage
### Token
Before you commit to a license, you are free to test all the features of the SDK for free. To start it, [send us a message](https://www.banuba.com/facear-sdk/face-filters#form).  
We will get back to you with the trial token.
You can store the token within the app.  

Feel free to [contact us](https://docs.banuba.com/face-ar-sdk-v1/support) if you have any questions.

### Banuba SDK Build
To get latest Banuba SDK build follow [Releases](https://github.com/Banuba/FaceAR-SDK-desktop-releases/releases) section in the [FaceAR-SDK-desktop-releases](https://github.com/Banuba/FaceAR-SDK-desktop-releases) repository.

### Getting Started

#### Windows

1. Clone the repository with submodules.
2. Copy `bnb_sdk.tar.gz/mac` (for Mac OS) or `bnb_sdk.zip/bin` with `bnb_sdk.zip/include` (for Windows) into `quickstart-desktop-cpp/bnb_sdk` dir.
3. Windows only: copy **contents** of `bnb_sdk.zip/resources` into `quickstart-desktop-cpp/resources`.
4. Copy and Paste your client token into appropriate section of [`quickstart-desktop-cpp/helpers/src/BanubaClientToken.hpp`](helpers/src/BanubaClientToken.hpp). 
5. Generate project files by executing next commands:

    **Windows x86 build:**	
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -A Win32 ..
    ```
    **Windows x64 build:**	
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -A x64 ..
    ```
6. Previous step will generate Visual Studio project for Windows. Open project in IDE.
7. Select target `single-image-processingr` or `realtime-camera-preview`.
8. Run build.

#### macOS

1. Clone the repository with submodules.
2. Copy `bnb_sdk.tar.gz/mac` into `quickstart-desktop-cpp/bnb_sdk` dir.
3. Copy and Paste your client token into appropriate section of [`quickstart-desktop-cpp/bnb_sdk_manager/BanubaClientToken.hpp`](bnb_sdk_manager/src/BanubaClientToken.hpp). 
4. Generate project files by executing next commands:
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -G Xcode ..
    ```
6. Previous step will generate Xcode project for MacOS and Visual Studio project for Windows. Open project in appropriate IDE on your platform.
7. Select target `single-image-processing`, `realtime-camera-preview` or `videostream-processing`.
8. Run build.

### Docs
You can find more info [here](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview).
