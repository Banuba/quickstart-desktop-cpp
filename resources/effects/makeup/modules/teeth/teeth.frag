#include <bnb/glsl.frag>
#include <bnb/lut.glsl>

BNB_IN(0)
vec4 var_uv;

BNB_DECLARE_SAMPLER_2D(0, 1, tex_camera);

BNB_DECLARE_SAMPLER_2D(2, 3, tex_mask);

BNB_DECLARE_SAMPLER_LUT(4, 5, tex_whitening);

void main()
{
    vec2 uv = var_uv.xy;

#ifdef BNB_VK_1
    uv.y = 1. - uv.y;
#endif
    vec4 camera = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_camera), uv);
    vec3 whitening = BNB_TEXTURE_LUT_SMALL(camera.rgb, BNB_PASS_SAMPLER_ARGUMENT(tex_whitening));

    float whitening_mask = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_mask), var_uv.zw).x;

    bnb_FragColor = vec4(whitening.rgb, whitening_mask);
}
