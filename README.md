[![](https://www.banuba.com/hubfs/Banuba_November2018/Images/Banuba%20SDK.png)](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview)

## Quick start examples for integrating [Banuba SDK on Desktop](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview) in C++ apps.  

Current example contains makeup and studio_light effects.

By default it runs makeup effect with options:

```js
function EnlargeEyes(strength){
    FaceMorph.eyes({enlargement: 0.5 * strength});
}

function FaceReshape(strength){
    FaceMorph.face({chin: -0.4 * strength});
    FaceMorph.lips({height: 0.8 * strength});
    FaceMorph.face({jaw_narrowing: 0.35 * strength, chin_narrowing: 0.35 * strength, narrowing: 0.25 * strength, cheekbones_narrowing: -0.5 * strength, forehead: 0.1 * strength});
    FaceMorph.nose({width: 0.3 * strength, length: 0.2 * strength, tip_width: -0.4 * strength});
    FaceMorph.lips({size: 0.3 * strength});
}

function LipsColor(color){
    Lips.color(color);
}

function SkinSmoothing(strength){
   Skin.softening(strength);
}

function BackgroundBlur(strength){
    Background.blur(strength);
}

function BrowsColor(color){
    Brows.color(color);
}

function BrowsImage(num){
    Brows.changeBrows(num)
}

function TeethWhitening(strength){
    Teeth.whitening(strength);
}

EnlargeEyes(1.0);
FaceReshape(1.0);
LipsColor("0.898 0.431 0.663 0.9");
SkinSmoothing(1.0);
BackgroundBlur(0.55);
Makeup.blushes("0.871 0.365 0.514 0.5");
Eyes.color("0.082 0.412 0.780 0.5");
BrowsColor("0.004 0.004 0.004 0.4");
BrowsImage(0);
TeethWhitening(0.5);
Eyes.whitening(0.2);
Makeup.eyeshadow("0.322 0.341 0.435 0.5")
```
If you want to run studio_light effect:
1) Go to `realtime-camera-preview/main.cpp`
2) Change the line 19 on the code below:

```cpp
sdk.load_effect("effects/studio_light", true);
```

If you want to run DebugWireframe effect:
1) Go to `realtime-camera-preview/main.cpp`
2) Change the line 19 on the code below:

```cpp
sdk.load_effect("effects/DebugWireframe", true);
```


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

1. Clone the repository with submodules.
2. Copy `bnb_sdk.tar.gz/mac` (for Mac OS) or `bnb_sdk.zip/bin` with `bnb_sdk.zip/include` (for Windows) into `quickstart-desktop-cpp/bnb_sdk` dir.
3. Windows only: copy **contents** of `bnb_sdk.zip/resources` into `quickstart-desktop-cpp/resources`.
4. Copy and Paste your client token into appropriate section of [`quickstart-desktop-cpp/bnb_sdk_manager/BanubaClientToken.hpp`](bnb_sdk_manager/src/BanubaClientToken.hpp). 
5. Generate project files by executing next commands:
    ##### Windows x86 build:	
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -A Win32 ..
    ```
    ##### Windows x64 build:	
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -A x64 ..
    ```
    ##### MacOS build:	
    ```
        cd %path_to_repository%
        git submodule update --init --recursive
        mkdir build
        cd build
        cmake -G Xcode ..
    ```
6. Previous step will generate Xcode project for MacOS and Visual Studio project for Windows. Open project in appropriate IDE on your platform.
7. Select target `single-image-processingr` or `realtime-camera-preview`.
8. Run build.

### Docs
You can find more info [here](https://docs.banuba.com/face-ar-sdk-v1/desktop/desktop_overview).

### Makeup Config

Config file of Makeup effect located here `resources/effects/makeup/config.js`

Feel free to add your custom code here using the [documentation](https://docs.banuba.com/face-ar-sdk-v1/effect_api/makeup)

