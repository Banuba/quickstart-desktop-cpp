#include "Utils.hpp"

#if BNB_OS_WINDOWS || BNB_OS_LINUX // See BanubaSdkManager.mm for OSX
std::string bnb::sdk_resources_path() {
    return ".";
}
#endif
