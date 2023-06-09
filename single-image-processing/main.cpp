#include "BanubaClientToken.hpp"

#include <BanubaSdkManager.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace bnb::interfaces;

int main()
{
    #if BNB_GL_BACKEND
    effect_player::set_render_backend(render_backend_type::opengl);
    #else
    effect_player::set_render_backend(render_backend_type::metal);
    #endif
    
    #if defined(__APPLE__)
    const auto result_path = (std::filesystem::temp_directory_path() / "bnb_result.jpg").string();
    #else
    const auto result_path = (std::filesystem::current_path() / "bnb_result.jpg").string();
    #endif
    
    {
        BanubaSdkManager sdk(
            "Single Image Processing",
            {BanubaSdkManager::sdk_resources_path(), BNB_RESOURCES_FOLDER},
            BNB_CLIENT_TOKEN);
        sdk.load_effect("effects/TrollGrandma", true);
        auto img = std::filesystem::path(BNB_RESOURCES_FOLDER) / "face720x1280.jpg";
        auto result = sdk.process_image(img);

        auto writing_result = stbi_write_jpg(result_path.c_str(), 720, 1280, 4, result.data.get(), 90);
        assert(writing_result != 0);
    }
    std::printf("Processing result was written to `%s`. \n", result_path.c_str());
    return 0;
}
