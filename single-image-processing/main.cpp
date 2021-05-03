#include "BanubaClientToken.hpp"

#include <BanubaSdkManager.hpp>

int main()
{
    BanubaSdkManager sdk(
        "Single Image Processing",
        {BanubaSdkManager::sdk_resources_path(), BNB_RESOURCES_FOLDER},
        BNB_CLIENT_TOKEN);
    sdk.load_effect("effects/Afro", true);
    auto img = path(BNB_RESOURCES_FOLDER) / "face720x1280.jpg";
    sdk.process_image(img); // Will right result near executable or in app bundle Resources
    return 0;
}
