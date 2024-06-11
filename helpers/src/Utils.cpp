#include "Utils.hpp"

#include <bnb/utils/defs.hpp>

#if !BNB_APPLE // See Utils.mm for MacOS
std::string bnb::sdk_resources_path() {
    return ".";
}
#endif
