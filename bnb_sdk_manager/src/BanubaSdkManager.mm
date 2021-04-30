#include "BanubaSdkManager.hpp"

#import <Foundation/Foundation.h>

std::string BanubaSdkManager::sdk_resources_path() 
{
    return std::string(NSBundle.mainBundle.bundlePath.UTF8String) +
        "/Contents/Frameworks/BanubaEffectPlayer.framework/Resources/bnb-resources/";
}
