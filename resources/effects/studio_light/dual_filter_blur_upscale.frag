#include <bnb/glsl.frag>

BNB_DECLARE_SAMPLER_2D(0, 1, s_upscale_tex);

BNB_IN(0)
vec4 var_uv;

void main()
{
    float sum =
        (1. / 6.) * (
            textureLod(BNB_SAMPLER_2D(s_upscale_tex), var_uv.zw, 0.).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.zw, 0., ivec2(-1, 0)).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.zw, 0., ivec2(0, -1)).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.zw, 0., ivec2(-1, -1)).x) +
        (1. / 12.) * (
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.xy, 0., ivec2(1, 0)).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.xy, 0., ivec2(-1, 0)).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.xy, 0., ivec2(0, 1)).x +
            textureLodOffset(BNB_SAMPLER_2D(s_upscale_tex), var_uv.xy, 0., ivec2(0, -1)).x);

    bnb_FragColor = vec4(sum, 0., 0., 0.);
}
