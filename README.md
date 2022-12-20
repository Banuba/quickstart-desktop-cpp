[![](https://www.banuba.com/hubfs/Banuba_November2018/Images/Banuba%20SDK.png)](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview)

## Quick start examples for integrating [Banuba SDK on Desktop](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview) in C++ apps.  

## [Requirements](https://docs.banuba.com/face-ar-sdk-v1/overview/system_requirements)

## Usage
### Token
Before you commit to a license, you are free to test all the features of the SDK for free. To start it, [send us a message](https://www.banuba.com/facear-sdk/face-filters#form).  
We will get back to you with the trial token.
You can store the token within the app.  

Feel free to [contact us](https://www.banuba.com/facear-sdk/face-filters#form) if you have any questions.

### Getting Started

1. Clone the repository
2. Copy `bnb_viewer_standalone/bnb_sdk/` into `quickstart-desktop-cpp/bnb_sdk` dir:
    `bnb_viewer_standalone/bnb_sdk/` => `quickstart-desktop-cpp/bnb_sdk`
3. For Windows and Linux: copy `bnb_viewer_standalone/resources/` files into `quickstart-desktop-cpp/resources` dir:
    `bnb_viewer_standalone/resources/` => `quickstart-desktop-cpp/resources`.
   For MacOS: copy `bnb_viewer_standalone/resources/mac` files into `quickstart-desktop-cpp/resources/mac` dir:
    `bnb_viewer_standalone/resources/mac` => `quickstart-desktop-cpp/resources/mac`.
4. Copy `bnb_viewer_standalone/third` files into `quickstart-desktop-cpp/third` dir:
    `bnb_viewer_standalone/third/` => `quickstart-desktop-cpp/third`
5. Copy and Paste your client token into appropriate section of [`bnb_sdk_manager/BanubaClientToken.hpp`](bnb_sdk_manager/BanubaClientToken.hpp). 

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

### Docs
You can find more info [here](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview).
