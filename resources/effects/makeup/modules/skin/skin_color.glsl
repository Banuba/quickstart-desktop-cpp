#include <bnb/glsl.frag>
#include <bnb/color_spaces.glsl>


vec4 skin_color(vec4 camera_color, BNB_DECLARE_SAMPLER_2D_ARGUMENT(tex_mask), vec2 mask_uv, vec4 target_skin_color)
{
    vec3 base_LCh = bnb_rgb_to_LCh(camera_color.rgb);
    vec3 target_LCh = bnb_rgb_to_LCh(target_skin_color.rgb);

    vec3 res_LCh = vec3(
        base_LCh.x,
        target_LCh.y,
        target_LCh.z);

    vec3 res_rgb = bnb_LCh_to_rgb(res_LCh);

    vec3 colored = mix(camera_color.rgb, res_rgb, target_skin_color.a);

    float r = colored.r;
    float g = colored.g;
    float b = colored.b;

    return vec4(r, g, b, 1.0);
}