#include "BanubaClientToken.hpp"

#include <BanubaSdkManager.hpp>

int main()
{
    BanubaSdkManager sdk("Single Image Processing", {BNB_RESOURCES_FOLDER}, BNB_CLIENT_TOKEN);
    sdk.load_effect("effects/Afro", true);
    auto img = boost::filesystem::path(BNB_RESOURCES_FOLDER) / "face720x1280.jpg";
    sdk.process_image(img);
    return 0;
}