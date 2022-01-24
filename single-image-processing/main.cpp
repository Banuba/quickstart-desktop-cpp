#include "BanubaClientToken.hpp"

#include <BanubaSdkManager.hpp>

using namespace bnb::interfaces;

int main()
{
    #if BNB_GL_BACKEND
    effect_player::set_render_backend(render_backend_type::opengl);
    #else
    effect_player::set_render_backend(render_backend_type::metal);
    #endif
    
    BanubaSdkManager sdk(
        "Single Image Processing",
        {BanubaSdkManager::sdk_resources_path(), BNB_RESOURCES_FOLDER},
        BNB_CLIENT_TOKEN);
    sdk.load_effect("effects/TrollGrandma", true);
    auto img = path(BNB_RESOURCES_FOLDER) / "face720x1280.jpg";
    sdk.process_image(img); // Will right result near executable or in app bundle Resources
    return 0;
}
